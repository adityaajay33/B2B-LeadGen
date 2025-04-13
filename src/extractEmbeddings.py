# src/extract.py
import sys
import json
from sentence_transformers import SentenceTransformer, util
import os

model = SentenceTransformer("all-MiniLM-L6-v2")

prompt = sys.argv[1]
embedding = model.encode(prompt)

print(",".join(str(x) for x in embedding))


