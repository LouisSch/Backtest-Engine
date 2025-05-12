# Backtest-Engine

A modular high-performance C++ backtesting engine featuring GBM + Jump tick simulation and multiple plug-and-play trading strategies. Containerized with Docker for easy execution.

---

🚧 **Work in Progress**
This project is actively being developped. Contributions, feedback and ideas are welcome!

---

## ✨ Features
- **GBM + Jump Tick Simulation**
  Simulates realistic tick-level price movements using Geometric Brownian Motion with jumps.
- **Pluggable Strategy Interface**
  Easily add or modify trading strategies (Mean Reversion, Breakout, Spread, etc...)
- **Multi-Strategy Execution**
  Backtest several strategies in parallel on the same tick stream.
- **Dockerized**
  Fully containerized, just build and run with Docker in a few seconds.
- **Performance Monitoring**
  Execution time logging to benchmark strategies and engine throughput.

---

## 📈 Strategues Included
- `MeanReversionSimple`
- `BreakoutStrategy`
- `SpreadStrategy`

Each strategy runs on the same tick series with configurable parameters and initial capital.

---

## 🚀 Getting Started

### Requirements
- Docker
- Git

### Build the image
```bash
docker build -t backtest-engine .
