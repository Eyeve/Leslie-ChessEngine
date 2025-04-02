import json
import chess
from concurrent.futures import ThreadPoolExecutor, as_completed
from functools import lru_cache
import sys

# Константы
DEPTH = 30
MAX_WORKERS = 4  # Оптимально для 4-ядерного процессора
CHUNK_SIZE = 1000  # Размер блока для обработки
BUFFER_SIZE = 10000  # Буфер записи

@lru_cache(maxsize=100000)
def fen_to_num(fen):
    board = chess.Board(fen)
    num = 0
    for square in chess.SQUARES:
        if piece := board.piece_at(square):
            ind = 64*6*int(piece.color) + 64*(piece.piece_type-1) + square
            num += 1 << ind  # Быстрее чем 2**ind
    return num

def process_line(line):
    try:
        item = json.loads(line)
        fen = item['fen']
        color = fen.split()[1]
        
        for eval in item.get('evals', []):
            if eval['depth'] >= DEPTH:
                pv = eval['pvs'][0]
                if 'mate' in pv:
                    score = pv['mate']
                    count = 32000 - score if score > 0 else -32000 - score
                else:
                    count = pv['cp']
                
                return json.dumps({
                    'fen': fen_to_num(fen),
                    'color': color,
                    'count': count
                }) + '\n'
    except Exception:
        return None

def process_chunk(chunk):
    results = []
    for line in chunk:
        if result := process_line(line):
            results.append(result)
    return results

def main():
    input_file = '/srv/chess/data.json'
    output_file = 'formed.json'
    
    with open(input_file, 'r') as f, \
         open(output_file, 'w', buffering=BUFFER_SIZE) as res, \
         ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        
        chunk = []
        futures = []
        processed = 0
        
        for line in f:
            chunk.append(line)
            if len(chunk) >= CHUNK_SIZE:
                futures.append(executor.submit(process_chunk, chunk))
                chunk = []
                
                if len(futures) >= MAX_WORKERS * 2:
                    for future in as_completed(futures):
                        res.writelines(future.result())
                        processed += len(future.result())
                        sys.stderr.write(f"\rОбработано: {processed}")
                    futures = []
        
        if chunk:
            futures.append(executor.submit(process_chunk, chunk))
        
        for future in as_completed(futures):
            res.writelines(future.result())
            processed += len(future.result())
            sys.stderr.write(f"\rОбработано: {processed}")

if __name__ == "__main__":
    main()