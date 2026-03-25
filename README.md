# Monte Carlo Options Pricer

A European options pricer built on a C++ simulation engine, exposed through a Python/Flask REST API, and served via a clean browser-based frontend.

---

## Features

- **Black-Scholes GBM simulation** — Geometric Brownian Motion path generation in C++
- **Call & Put pricing** — European-style options with configurable parameters
- **Statistical output** — Option price, standard error, and 95% confidence interval
- **Convergence & payoff charts** — Visualise simulation quality and payoff profiles
- **Run history** — In-session table of all pricing runs
- **REST API** — Simple JSON endpoint for programmatic access

---

## Project Structure

```
monte-carlo-pricer/
├── core/
│   ├── payoffs.h / payoffs.cpp      # Call & Put payoff functions
│   ├── pricer.h  / pricer.cpp       # Monte Carlo pricing engine (MCResult)
│   └── simulation.h / simulation.cpp # GBM path simulation
├── frontend/
│   └── index.html                   # Single-file browser UI (HTML + CSS + JS)
├── main.cpp                         # CLI entry point for the C++ binary
├── server.py                        # Flask server — bridges frontend ↔ C++ binary
├── Makefile                         # Build the C++ binary
└── .gitignore
```

---

## Requirements

| Tool | Version |
|------|---------|
| g++ (MinGW on Windows / GCC on Linux) | C++17 capable |
| Python | 3.8+ |
| Flask | Any recent version |

Install Python dependencies:

```bash
pip install flask
```

---

## Build

Compile the C++ pricing engine with:

```bash
make
```

This produces `mc_pricer.exe` (Windows) or `mc_pricer` (Linux/macOS) in the project root.

To clean build artifacts:

```bash
make clean
```

---

## Run

Start the Flask development server:

```bash
python server.py
```

Then open your browser and navigate to:

```
http://localhost:5000
```

---

## CLI Usage

The compiled binary can also be used directly:

```bash
./mc_pricer <S> <K> <r> <sigma> <T> <paths> <time_steps> <option_type>
```

| Argument | Description | Default |
|---|---|---|
| `S` | Spot price | `100.0` |
| `K` | Strike price | `100.0` |
| `r` | Risk-free rate (e.g. `0.05` = 5%) | `0.05` |
| `sigma` | Volatility (e.g. `0.20` = 20%) | `0.20` |
| `T` | Time to expiry in years | `1.0` |
| `paths` | Number of simulation paths | `100000` |
| `time_steps` | Time steps per path | `252` |
| `option_type` | `call` or `put` | `call` |

**Example:**

```bash
./mc_pricer 100 105 0.05 0.20 1.0 100000 252 call
```

**Output:**

```
price=7.964123
std_error=0.025341
ci_lower=7.914455
ci_upper=8.013791
```

---

## API Reference

### `POST /api/price`

Price an option via JSON.

**Request body:**

```json
{
  "S": 100,
  "K": 105,
  "r": 0.05,
  "sigma": 0.20,
  "T": 1.0,
  "paths": 100000,
  "time_steps": 252,
  "option_type": "call"
}
```

**Response:**

```json
{
  "price": 7.964123,
  "std_error": 0.025341,
  "ci_lower": 7.914455,
  "ci_upper": 8.013791,
  "inputs": {
    "S": 100, "K": 105, "r": 0.05, "sigma": 0.2,
    "T": 1.0, "paths": 100000, "time_steps": 252, "option_type": "call"
  }
}
```

**Validation rules:**
- `paths` must be between 1,000 and 1,000,000
- `S`, `K`, `T`, `sigma` must all be positive
- `option_type` must be `"call"` or `"put"`

---

## How It Works

1. The **browser frontend** collects parameters and sends a `POST /api/price` request to Flask.
2. The **Flask server** validates inputs and spawns the compiled `mc_pricer` binary as a subprocess.
3. The **C++ engine** simulates thousands of GBM price paths, computes the discounted average payoff, and prints results as `key=value` pairs.
4. Flask parses the output and returns a JSON response to the frontend.
5. The **frontend** renders the price, confidence interval bar, convergence chart, and payoff profile.

---

## License

MIT
