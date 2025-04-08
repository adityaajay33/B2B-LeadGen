# src/extract.py
import sys
import json
from sentence_transformers import SentenceTransformer, util
import os

model = SentenceTransformer("all-MiniLM-L6-v2")

def load_categories_from_txt(file_path):
    categories = {}
    current_key = None
    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            if line.endswith(":"):
                current_key = line[:-1]
                categories[current_key] = []
            elif current_key:
                categories[current_key].append(line)
    return categories

categories = load_categories_from_txt(os.path.join(os.path.dirname(__file__), "categories.txt"))

prompt = sys.argv[1]
prompt_embedding = model.encode(prompt, convert_to_tensor=True)

results = {}
for category, options in categories.items():
    option_embeddings = model.encode(options, convert_to_tensor=True)
    similarities = util.cos_sim(prompt_embedding, option_embeddings)[0]
    best_match_idx = similarities.argmax()
    best_match = options[best_match_idx]
    confidence = similarities[best_match_idx].item()
    results[category] = {"match": best_match, "confidence": round(confidence, 3)}

with open("temp_output.json", "w") as f:
    json.dump(results, f)
