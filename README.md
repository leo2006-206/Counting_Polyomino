# Counting Polyomino

## Overview

**Counting Polyomino** is a C++ project focused on the **enumeration, analysis, and performance evaluation of polyominoes** — connected shapes formed by joining unit squares edge-to-edge.

The project explores both the **algorithmic side** (correct generation, symmetry handling, counting) and the **systems side** (performance, memory layout and multithreading).

The goal is not just to count polyominoes, but to **understand what is preformance engineering and improve C++ skill**.

Example image (Rotation 0, Reflection 0):

<img src="./PolyominoImage/8Poly_D4/Int/R0.png" alt="drawing" style="width:600px;"/>
<img src="./PolyominoImage/8Poly_D4/Int/RF0.png" alt="drawing" style="width:600px;"/>

---

## What This Project Does

At a high level, the project:

- Enumerates polyominoes of increasing order (number of cells)
- Eliminates duplicates caused by **translation, rotation, and reflection**
- Analyzes structural properties of generated polyominoes
- Compares different enumeration and counting strategies
- Generates data and visual representations for further study

This makes the project suitable for:
- Algorithm research / experimentation
- Performance engineering practice
- Learning advanced C++

---

## Core Concepts

The project works with several fundamental ideas:

- **Polyomino representation**  
  Efficient internal representations of cell connectivity and shape identity.

- **Canonicalization & symmetry**  
  Handling rotations and reflections so that each polyomino is counted exactly once.

- **Counting & analysis**  
  Producing counts, statistics, and structured datasets for further processing.

- **Concurrency & scalability**  
  Exploring how enumeration behaves under multi-threading and shared-state contention.

---

## Project Structure

```

Counting_Polyomino/
├── Header/                # Core implementation
├── PolyominoEnumerate/    # Enumeration
├── PolyominoAnalyse/      # Analysis and statistics
├── PolyominoImage/        # Shape visualization utilities
├── Draw_Polyomino.py      # Python helper for rendering polyominoes
├── CSV_Data/              # Generated datasets
├── UnitSpeedCompare/      # Performance benchmarks
├── SmallTest/             # External experiment
├── text/                  # Notes and documentation

````

---

## Performance Focus

A major aspect of this project is **measuring and understanding performance**:

- Comparison between:
  - Data structure and Algorithm
  - Library usage
  - Object encoding
- Investigation of:
  - CPU utility like (vectorised asm, pipling, branch prediction, cache effect...)
  - Algorithmic think
  - Performance measure and comparison
  - C++ tool and optimization 
- Timing and benchmarking of different approaches
