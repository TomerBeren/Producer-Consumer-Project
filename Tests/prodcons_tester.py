import subprocess
import os
import time
import shutil

# Constants
CONFIGS = {
    "config1": 70,
    "config2": 65,
    "config3": 15,
    "config4": 251
}  # Configuration names and their expected line counts
TIMEOUT = 300  # 5 minutes timeout in seconds
OUTPUT_FILE = "test_results.txt"

# Function to check the order of production
def check_order(filename):
    producer_counts = {}
    with open(filename, 'r') as f:
        for line in f:
            if line.startswith("Producer"):
                parts = line.split()
                producer = int(parts[1])
                product_type = parts[2]
                count = int(parts[3])

                if producer not in producer_counts:
                    producer_counts[producer] = {"SPORTS": -1, "NEWS": -1, "WEATHER": -1}

                if producer_counts[producer][product_type] != count - 1:
                    return False
                producer_counts[producer][product_type] = count
    return True

# Function to compile the program using Makefile
def compile_program():
    if not shutil.which("make"):
        return False
    result = subprocess.run(["make", "-C", ".."], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return result.returncode == 0

# Main testing function
def main():
    # Compile the program
    with open(OUTPUT_FILE, 'w') as output:
        if not compile_program():
            output.write("Compilation failed or make not found.\n")
            return

        for config, expected_lines in CONFIGS.items():
            config_output_file = f"{config}_output.txt"
            
            # Run the program with the configuration file with a timeout to prevent deadlock
            try:
                start_time = time.time()
                result = subprocess.run(["../ProdCons", f"./{config}"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=TIMEOUT)
                elapsed_time = time.time() - start_time
                output_text = result.stdout.decode()
                exit_status = result.returncode

                # Write the output to the specific config output file
                with open(config_output_file, 'w') as f:
                    f.write(output_text)

                # Check if the program entered a deadlock
                if elapsed_time >= TIMEOUT:
                    output.write(f"Test {config} FAILED: Program entered a deadlock.\n")
                    break

                # Verify the output
                actual_lines = output_text.strip().split('\n')
                num_producers = len(set([int(line.split()[1]) for line in actual_lines if line.startswith("Producer")]))
                errors = []

                if len(actual_lines) != expected_lines:
                    errors.append(f"Expected {expected_lines} lines, got {len(actual_lines)}")
                if "DONE" not in actual_lines[-1]:
                    errors.append("Missing DONE at the end")
                if not check_order(config_output_file):
                    errors.append("Incorrect order of production")

                if errors:
                    for error in errors:
                        output.write(f"Test {config} FAILED: {error}\n")
                else:
                    output.write(f"Test {config} PASSED\n")

            except subprocess.TimeoutExpired:
                output.write(f"Test {config} FAILED: Program entered a deadlock.\n")
                break
            except subprocess.CalledProcessError:
                output.write(f"Test {config} FAILED: Compilation error or runtime error.\n")
                break

if __name__ == "__main__":
    main()
