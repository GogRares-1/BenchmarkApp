import tkinter as tk
from tkinter import ttk
import subprocess
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import threading


EXE_PATH = r"C:\Users\admin\Desktop\Programe\Facultate\Anu3\Sem1\SCS\PROIECT_SCS\BenchmarkApp\x64\Debug\BenchmarkApp.exe"

def run_benchmark(args_list):
    try:
        result = subprocess.run(
            [EXE_PATH] + args_list,
            capture_output=True,
            text=True
        )
        return result.stdout if result.stdout else result.stderr
    except Exception as e:
        return f"Error running executable:\n{e}"

def show_output(text):
    output_box.delete("1.0", tk.END)
    output_box.insert(tk.END, text)

def show_loading_window():
    loading = tk.Toplevel(root)
    loading.title("Please wait")
    loading.geometry("200x100")
    loading.resizable(False, False)
    loading.transient(root)
    loading.grab_set()

    label = tk.Label(loading, text="Loading", font=("Arial", 14))
    label.pack(expand=True)

    running = {"value": True}

    def animate(step=0):
        if not running["value"]:
            return
        label.config(text="Loading" + "." * step)
        root.after(500, animate, (step + 1) % 4)

    animate()

    def stop():
        running["value"] = False
        loading.destroy()

    return stop

def run_in_background(args_list):
    stop_loading = show_loading_window()

    def task():
        output = run_benchmark(args_list)
        root.after(0, lambda: finish_task(output, stop_loading))

    threading.Thread(target=task, daemon=True).start()


def finish_task(output, stop_loading):
    stop_loading()
    show_output(output)

def on_view_system_info():
    run_in_background(["--system"])

def on_run_all():
    run_in_background(["--all"])

def on_run_specific():
    test = test_choice.get()
    if not test:
        show_output("Please select a test")
        return

    mapping = {
        "Arithmetic Test": ["--cpu"],
        "Sorting Test": ["--sort"],
        "Multithread Test": ["--multithread"],
        "Memory Test": ["--memory"],
        "Disk Test": ["--disk"],
        "FLOPS Test": ["--flops"],
        "System Information": ["--system"]
    }

    run_in_background(mapping[test])

def on_save_results():
    text = output_box.get("1.0", tk.END).strip()
    if not text:
        show_output("Nothing to save\n")
        return

    safe_text = text.replace("\n", "\\n")

    result = run_benchmark(["--save", safe_text])
    show_output(result)

def clear_graph():
    for widget in graph_frame.winfo_children():
        widget.destroy()

def show_mt_graph():
    stop_loading = show_loading_window()

    def task():
        output = run_benchmark(["--multithread"])
        root.after(0, lambda: finish_mt_graph(output, stop_loading))

    threading.Thread(target=task, daemon=True).start()


def finish_mt_graph(output, stop_loading):
    stop_loading()
    show_output(output)
    clear_graph()

    threads = []
    times = []

    for line in output.splitlines():
        if line.startswith("GRAPH_MT"):
            parts = line.split()
            threads.append(int(parts[1].split("=")[1]))
            times.append(float(parts[2].split("=")[1]))

    if not threads:
        return

    fig = Figure(figsize=(6, 4), dpi=100)
    ax = fig.add_subplot(111)

    ax.plot(threads, times, marker='o')
    ax.set_title("Multithread Performance Scaling")
    ax.set_xlabel("Threads")
    ax.set_ylabel("Time (sec)")
    ax.grid(True)

    canvas = FigureCanvasTkAgg(fig, master=graph_frame)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

def show_memcpy_graph():
    stop_loading = show_loading_window()

    def task():
        output = run_benchmark(["--memory"])
        root.after(0, lambda: finish_memcpy_graph(output, stop_loading))

    threading.Thread(target=task, daemon=True).start()


def finish_memcpy_graph(output, stop_loading):
    stop_loading()
    show_output(output)
    clear_graph()

    block_sizes = []
    speeds = []

    for line in output.splitlines():
        if line.startswith("GRAPH_MEMCPY"):
            parts = line.split()
            block_sizes.append(int(parts[1].split("=")[1]))
            speeds.append(float(parts[2].split("=")[1]))

    if not block_sizes:
        return

    fig = Figure(figsize=(6, 4), dpi=100)
    ax = fig.add_subplot(111)

    ax.plot(block_sizes, speeds, marker='o')
    ax.set_xscale("log")
    ax.set_title("Memcpy Cache-Level Memory Performance")
    ax.set_xlabel("Block Size (bytes)")
    ax.set_ylabel("Speed (GB/s)")
    ax.grid(True)

    canvas = FigureCanvasTkAgg(fig, master=graph_frame)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

root = tk.Tk()
root.title("Benchmark GUI")
root.geometry("1000x650")

top = tk.Frame(root)
top.pack(pady=10)

tk.Button(top, text="View System Information", command=on_view_system_info).pack(side=tk.LEFT, padx=5)
tk.Button(top, text="Run All Tests", command=on_run_all).pack(side=tk.LEFT, padx=5)

test_choice = ttk.Combobox(
    top,
    values=[
        "Arithmetic Test",
        "Sorting Test",
        "Multithread Test",
        "Memory Test",
        "Disk Test",
        "FLOPS Test",
        "System Information"
    ],
    state="readonly",
    width=25
)
test_choice.pack(side=tk.LEFT, padx=5)

tk.Button(top, text="Run Specific Test", command=on_run_specific).pack(side=tk.LEFT, padx=5)
tk.Button(top, text="Save Test Results", command=on_save_results).pack(side=tk.LEFT, padx=5)

tk.Button(top, text="Graph: Multithread", command=show_mt_graph).pack(side=tk.LEFT, padx=5)
tk.Button(top, text="Graph: memcpy()", command=show_memcpy_graph).pack(side=tk.LEFT, padx=5)

output_box = tk.Text(root, wrap=tk.NONE, height=22)
output_box.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
graph_frame = tk.Frame(root)
graph_frame.pack(fill=tk.BOTH, expand=True)

scroll_y = tk.Scrollbar(output_box, orient=tk.VERTICAL, command=output_box.yview)
scroll_y.pack(side=tk.RIGHT, fill=tk.Y)
output_box.config(yscrollcommand=scroll_y.set)

root.mainloop()