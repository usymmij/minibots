import socket
import tkinter as tk

IP = "192.168.4.1"
PORT = 1234
class Status:
    def __init__(self): 
        self.l = 0
        self.r = 0
status = Status()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

main = tk.Tk()
label = tk.Label(text="use wasd for control\nq to exit", background="#34A2FE",)
label.pack()

main.config(height=500, width=500)
label.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

def move(event):
    global status
    ch = event.char.lower()
    ch = ord(ch)
    match ch:
        case 119: # ord('w')
            status.l += 1
            status.r += 1
        case 97: # ord('a')
            status.l -= 1
            status.r += 1
        case 115: # ord('s')
            status.l -= 1
            status.r -= 1
        case 100: # ord('d')
            status.l += 1
            status.r -= 1
        case _:
            pass
def stopmove(event):
    global status
    ch = event.keysym.lower()
    ch = ord(ch)
    match ch:
        case 119: # ord('w')
            status.l -= 1
            status.r -= 1
        case 97: # ord('a')
            status.l += 1
            status.r -= 1
        case 115: # ord('s')
            status.l += 1
            status.r += 1
        case 100: # ord('d')
            status.l -= 1
            status.r += 1
        case _:
            pass

def update(*args):
    global status
    right = status.r
    right = 0 if right == 0 else (1 if right > 0 else -1) # [-1, 1]
    right %= 4 # 0, 1, 3(backward)
    left = status.l
    left = 0 if left == 0 else (1 if left > 0 else -1)
    left %= 4

    sock.sendto(bytes(chr((left << 2) | right), 'utf-8'), (IP, PORT))
    main.after(50,update, status)

main.bind("<KeyPress-W>", move, status)
main.bind("<KeyPress-w>", move, status)
main.bind("<KeyPress-A>", move, status)
main.bind("<KeyPress-a>", move, status)
main.bind("<KeyPress-S>", move, status)
main.bind("<KeyPress-s>", move, status)
main.bind("<KeyPress-D>", move, status)
main.bind("<KeyPress-d>", move, status)
main.bind("<KeyRelease-W>", stopmove, status)
main.bind("<KeyRelease-w>", stopmove, status)
main.bind("<KeyRelease-A>", stopmove, status)
main.bind("<KeyRelease-a>", stopmove, status)
main.bind("<KeyRelease-S>", stopmove, status)
main.bind("<KeyRelease-s>", stopmove, status)
main.bind("<KeyRelease-D>", stopmove, status)
main.bind("<KeyRelease-d>", stopmove, status)
main.bind("<Q>", lambda e: exit(0))
main.bind("<q>", lambda e: exit(0))
main.after(50, update)

while True:
    main.mainloop()

