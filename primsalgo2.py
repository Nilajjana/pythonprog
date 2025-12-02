import pygame
import random
import sys
import threading
import collections

# ensure your pybind11 module path
sys.path.append("/home/nilaj-jana/pythonprog")
import maze_cpp  # C++ pybind11 module

pygame.init()
pygame.font.init()

# ---------- Config ----------
WINDOW_WIDTH = 1000
WINDOW_HEIGHT = 700
FPS = 60

BG_COLOR = (10, 10, 10)
CELL_COLOR = (255, 255, 255)     # White = Wall/Unvisited
PATH_COLOR = (238, 238, 238)     # Light grey = Path/Carved
UI_BG = (30, 30, 30)
UI_TEXT = (220, 220, 220)
BTN_COLOR = (60, 60, 60)

FONT = pygame.font.SysFont("consolas", 16)
TITLE_FONT = pygame.font.SysFont("consolas", 20, bold=True)

# ---------- Helper UI ----------
class InputBox:
    def __init__(self, rect, text="", placeholder=""):
        self.rect = pygame.Rect(rect)
        self.text = text
        self.placeholder = placeholder
        self.active = False

    def handle_event(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            self.active = self.rect.collidepoint(event.pos)
        if self.active and event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN:
                self.active = False
                return "enter"
            elif event.key == pygame.K_BACKSPACE:
                self.text = self.text[:-1]
            else:
                if event.unicode:
                    self.text += event.unicode
        return None

    def draw(self, screen):
        color = (50, 50, 50) if self.active else (40, 40, 40)
        pygame.draw.rect(screen, color, self.rect, border_radius=4)
        txt = self.text if self.text else self.placeholder
        txt_col = UI_TEXT if self.text else (150, 150, 150)
        surf = FONT.render(txt, True, txt_col)
        screen.blit(surf, (self.rect.x + 6, self.rect.y + (self.rect.h - surf.get_height()) // 2))
        pygame.draw.rect(screen, (90, 90, 90), self.rect, 1, border_radius=4)

    def get(self):
        return self.text.strip()

class Button:
    def __init__(self, rect, label):
        self.rect = pygame.Rect(rect)
        self.label = label

    def draw(self, screen):
        pygame.draw.rect(screen, BTN_COLOR, self.rect, border_radius=6)
        surf = FONT.render(self.label, True, UI_TEXT)
        screen.blit(surf, (self.rect.x + (self.rect.w - surf.get_width()) // 2,
                           self.rect.y + (self.rect.h - surf.get_height()) // 2))

    def handle_event(self, event):
        return event.type == pygame.MOUSEBUTTONDOWN and self.rect.collidepoint(event.pos)

# ---------- Maze Class ----------
class Maze:
    def __init__(self, logic_rows, logic_cols, win_w, win_h, seed=None):
        # Logic rows/cols from UI
        self.logic_rows = logic_rows
        self.logic_cols = logic_cols

        # Expanded grid dimensions (matches C++ code)
        self.rows = logic_rows * 2 + 1
        self.cols = logic_cols * 2 + 1

        self.win_w = win_w
        self.win_h = win_h

        if seed is not None:
            try:
                random.seed(int(seed))
            except:
                pass

        self.cell_size = max(1, min(self.win_w // self.cols, (self.win_h - ui_height) // self.rows))
        self.surface = pygame.Surface((self.win_w, self.win_h - ui_height))
        self.surface.fill(BG_COLOR)

        # True = Wall/Unvisited, False = Carved
        self.grid = [[True for _ in range(self.cols)] for _ in range(self.rows)]
        self.generated = False
        self.stop_flag = False

    def recompute_size(self, win_w, win_h):
        self.win_w = win_w
        self.win_h = win_h
        self.cell_size = max(1, min(self.win_w // self.cols, (self.win_h - ui_height) // self.rows))
        self.surface = pygame.Surface((self.win_w, self.win_h - ui_height))
        self.surface.fill(BG_COLOR)
        self.redraw()

    def redraw(self):
        for r in range(self.rows):
            for c in range(self.cols):
                self.draw_cell(r, c)

    def clear_surface(self):
        self.surface.fill(BG_COLOR)

    def draw_cell(self, r, c):
        color = PATH_COLOR if not self.grid[r][c] else CELL_COLOR
        pygame.draw.rect(self.surface, color,
                         pygame.Rect(c * self.cell_size, r * self.cell_size, self.cell_size, self.cell_size))

    def generate_grid_generator(self):
        self.clear_surface()
        for r in range(self.rows):
            for c in range(self.cols):
                self.draw_cell(r, c)
                yield (r, c)
        self.generated = True

# ---------- UI Setup ----------
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), pygame.RESIZABLE)
pygame.display.set_caption("Maze Generator (Pygame + C++)")
clock = pygame.time.Clock()
ui_height = 70

inp_rows = InputBox((10, 10, 110, 28), placeholder="rows (required)")
inp_cols = InputBox((130, 10, 130, 28), placeholder="cols (required)")
inp_seed = InputBox((270, 10, 160, 28), placeholder="seed (optional)")
btn_start = Button((440, 10, 90, 28), "Start")
btn_stop = Button((540, 10, 90, 28), "Stop")
btn_quit = Button((640, 10, 90, 28), "Quit")

status_text = "Fill rows & cols, then Start."

maze = None
grid_gen = None
updates_queue = collections.deque()

def start_generation():
    global maze, grid_gen, status_text
    try:
        r = int(inp_rows.get())
        c = int(inp_cols.get())
    except:
        status_text = "Invalid row/col input."
        return
    seed = inp_seed.get()
    seed = int(seed) if seed else None
    maze = Maze(r, c, screen.get_width(), screen.get_height(), seed)
    grid_gen = maze.generate_grid_generator()
    maze.generated = False
    maze.stop_flag = False
    status_text = f"Generating grid {r}x{c}..."

def start_carving(seed_r, seed_c):
    def worker():
        seed_idx = seed_r * maze.cols + seed_c
        updates = maze_cpp.generate(maze.logic_rows, maze.logic_cols, seed_idx)
        for idx, is_wall in updates:
            updates_queue.append((idx, is_wall))
    t = threading.Thread(target=worker, daemon=True)
    t.start()
    return t

# ---------- Main Loop ----------
running = True
while running:
    clock.tick(FPS)
    screen.fill(BG_COLOR)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.VIDEORESIZE and maze:
            screen = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)
            maze.recompute_size(event.w, event.h)
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if btn_start.handle_event(event): start_generation()
            if btn_stop.handle_event(event) and maze: maze.stop_flag = True
            if btn_quit.handle_event(event): running = False
            for inp in (inp_rows, inp_cols, inp_seed): inp.handle_event(event)
            if maze and maze.generated:
                mx, my = event.pos
                if my > ui_height:
                    c, r = mx // maze.cell_size, (my - ui_height) // maze.cell_size
                    if 0 <= r < maze.rows and 0 <= c < maze.cols:
                        start_carving(r, c)
        elif event.type == pygame.KEYDOWN:
            for inp in (inp_rows, inp_cols, inp_seed):
                res = inp.handle_event(event)
                if res == "enter": start_generation()
            if event.key == pygame.K_RETURN: start_generation()
            if event.key == pygame.K_ESCAPE: running = False

    # UI
    pygame.draw.rect(screen, UI_BG, pygame.Rect(0, 0, screen.get_width(), ui_height))
    title = TITLE_FONT.render("Maze Generator (Pygame + C++)", True, UI_TEXT)
    screen.blit(title, (10, ui_height - 36))
    status = FONT.render(status_text, True, UI_TEXT)
    screen.blit(status, (10, ui_height - 18))
    inp_rows.draw(screen); inp_cols.draw(screen); inp_seed.draw(screen)
    btn_start.draw(screen); btn_stop.draw(screen); btn_quit.draw(screen)

    # Draw initial grid
    if maze and not maze.generated:
        try:
            while True: next(grid_gen)
        except StopIteration:
            maze.generated = True
            status_text = f"Grid {maze.logic_rows}x{maze.logic_cols} ready. Click to carve."

    # Apply updates
    while updates_queue:
        idx, is_wall = updates_queue.popleft()
        r, c = divmod(idx, maze.cols)  # works now because maze.cols = expanded cols
        maze.grid[r][c] = is_wall
        maze.draw_cell(r, c)

    if maze: screen.blit(maze.surface, (0, ui_height))
    pygame.display.flip()

pygame.quit()
