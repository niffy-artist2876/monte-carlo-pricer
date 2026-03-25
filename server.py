from flask import Flask, request, jsonify, send_from_directory
import subprocess
import os
import sys

app = Flask(__name__, static_folder="frontend")

BINARY = os.path.join(os.path.dirname(__file__), "mc_pricer")
if sys.platform == "win32":
    BINARY += ".exe"
# Fallback: try without .exe (MinGW sometimes omits it)
if not os.path.exists(BINARY):
    BINARY = os.path.join(os.path.dirname(__file__), "mc_pricer")


@app.route("/")
def index():
    return send_from_directory("frontend", "index.html")


@app.route("/api/price", methods=["POST"])
def price():
    data = request.get_json(force=True)

    # Extract and validate parameters
    try:
        S           = float(data.get("S", 100))
        K           = float(data.get("K", 100))
        r           = float(data.get("r", 0.05))
        sigma       = float(data.get("sigma", 0.20))
        T           = float(data.get("T", 1.0))
        paths       = int(data.get("paths", 100000))
        time_steps  = int(data.get("time_steps", 252))
        option_type = str(data.get("option_type", "call")).lower()
    except (ValueError, TypeError) as e:
        return jsonify({"error": f"Invalid parameter: {e}"}), 400

    if option_type not in ("call", "put"):
        return jsonify({"error": "option_type must be 'call' or 'put'"}), 400
    if paths < 1000 or paths > 1_000_000:
        return jsonify({"error": "paths must be between 1,000 and 1,000,000"}), 400
    if T <= 0 or sigma <= 0 or S <= 0 or K <= 0:
        return jsonify({"error": "S, K, T, sigma must all be positive"}), 400

    # Call the C++ binary
    args = [BINARY, str(S), str(K), str(r), str(sigma), str(T),
            str(paths), str(time_steps), option_type]
    try:
        result = subprocess.run(args, capture_output=True, text=True, timeout=30)
    except FileNotFoundError:
        return jsonify({"error": "mc_pricer binary not found. Please compile first."}), 500
    except subprocess.TimeoutExpired:
        return jsonify({"error": "Pricing timed out"}), 500

    if result.returncode != 0:
        return jsonify({"error": result.stderr or "Binary returned non-zero exit code"}), 500

    # Parse key=value output
    output = {}
    for line in result.stdout.strip().splitlines():
        if "=" in line:
            k, v = line.split("=", 1)
            output[k.strip()] = float(v.strip())

    required = {"price", "std_error", "ci_lower", "ci_upper"}
    if not required.issubset(output):
        return jsonify({"error": "Unexpected output from binary", "raw": result.stdout}), 500

    return jsonify({
        "price":     round(output["price"], 6),
        "std_error": round(output["std_error"], 6),
        "ci_lower":  round(output["ci_lower"], 6),
        "ci_upper":  round(output["ci_upper"], 6),
        "inputs": {
            "S": S, "K": K, "r": r, "sigma": sigma,
            "T": T, "paths": paths, "time_steps": time_steps,
            "option_type": option_type
        }
    })


if __name__ == "__main__":
    print("Monte Carlo Pricer API running at http://localhost:5000")
    app.run(debug=True, port=5000)
