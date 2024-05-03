import serial
import argparse
import datetime

def parse_args():
    parser = argparse.ArgumentParser(description="Serial Port Echo with Match")
    parser.add_argument('-b', '--baud', type=int, default=9600,
                        help='Specify the baud rate. Default is 9600.')
    parser.add_argument('-p', '--port', type=str, required=True,
                        help='Specify the serial port. E.g., COM3 or /dev/ttyUSB0')
    parser.add_argument('-f', '--file', type=str, required=True,
                        help='Specify the file path for matching content.')
    return parser.parse_args()

def read_file_contents(file_path):
    with open(file_path, 'r') as file:
        # Read and store both the direct string and its hex-encoded version.
        lines = file.readlines()
        return [(line.strip(), line.strip().encode().hex().upper()) for line in lines]

def is_match(received_data, matching_contents):

    for direct_string, hex_string in matching_contents:
        # Directly compare the received data as a string or as a hex-encoded string
        if received_data == direct_string or received_data == hex_string.upper():
            return True
    return False


def print_file_contents(matching_contents):
    print("File content and corresponding hex values:")
    for direct_string, hex_string in matching_contents:
        print(f"Original: {direct_string}, Hex: {hex_string.upper()}")

def main():
    args = parse_args()

    serial_port = args.port
    baud_rate = args.baud
    file_path = args.file

    matching_contents = read_file_contents(file_path)
    # 打印文件内容的原始字符串和其16进制大写版本
    print_file_contents(matching_contents)
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=2)
        print(f"Connected to {serial_port} at {baud_rate} baud.")

        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').strip()
                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                
                if is_match(data, matching_contents):
                    print(f"{timestamp} Received matching data: {data}")
                else:
                    print(f"{timestamp} Received (no match): {data}")

    except serial.SerialException as e:
        print("Serial exception:", e)
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    finally:
        if ser is not None and ser.is_open:
            ser.close()
            print("Serial port closed.")

if __name__ == "__main__":
    main()

