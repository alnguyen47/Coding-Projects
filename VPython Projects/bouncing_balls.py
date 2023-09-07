from vpython import *

# Create Wall(s)
side = 24

wallR = box(pos=vector(side / 2, 0, 0), size=vec(0.1, side, side * 2), color=color.purple)
wallL = box(pos=vector(-side / 2, 0, 0), size=vec(0.1, side, side * 2), color=color.yellow)
wallT = box(pos=vector(0, side / 2, 0), size=vec(side, 0, side * 2), color=color.green)
wallB = box(pos=vector(0, -side / 2, 0), size=vec(side, 0, side * 2), color=color.red)
wallF = box(pos=vector(0, 0, -side), size=vec(side, side, 0), color=color.blue, opacity=0.2)
wallD = box(pos=vector(0, 0, side), size=vec(side, side, 0), color=color.white, opacity=0.2)

# Create Ball
ball = sphere(pos=vec(3, -1, 7), color=color.green, radius=2)
ball2 = sphere(pos=vec(1, -1, 9), color=color.red, radius=2)
ball.momentum = vector(3, 3, -12)
ball2.momentum = vector (-2, -1, 9)
mass = 0.864
F = vector(0, -9.8, 0) * mass
realBounce = (side - 0.1) / 2 - ball.radius

# Time interval
t = 0
dt = 0.005

# Vector line
ball.trail = curve(color=ball.color)
ball2.trail = curve(color=ball2.color)
vscale = 0.1

while (t < 10):
    rate(200)

    # Move balls
    ball.pos = ball.pos + (ball.momentum / mass) * dt
    ball.momentum = ball.momentum + F * dt

    ball2.pos = ball2.pos + (ball2.momentum / mass) * dt
    ball2.momentum = ball2.momentum + F * dt

    # check for collisions with the walls
    # right wall
    if ball.pos.x > realBounce:
        ball.momentum.x = -ball.momentum.x  # reflect momentum in x dir

    if ball2.pos.x > realBounce:
        ball2.momentum.x = -ball2.momentum.x  # reflect momentum in x dir

    # left wall
    if ball.pos.x < -realBounce:
        ball.momentum.x = -ball.momentum.x

    if ball2.pos.x < -realBounce:
        ball2.momentum.x = -ball2.momentum.x

    # check for collisions with the walls
    # top wall
    if ball.pos.y > realBounce:
        ball.momentum.y = -ball.momentum.y  # reflect momentum in y dir

    if ball2.pos.y > realBounce:
        ball2.momentum.y = -ball2.momentum.y  # reflect momentum in y dir

    # bottom wall
    if ball.pos.y < -realBounce:
        ball.momentum.y = -ball.momentum.y

    if ball2.pos.y < -realBounce:
        ball2.momentum.y = -ball2.momentum.y

    # front wall
    if ball.pos.z > side - ball.radius:
        ball.momentum.z = -ball.momentum.z  # reflect momentum in z dir

    if ball2.pos.z > side - ball2.radius:
        ball2.momentum.z = -ball2.momentum.z  # reflect momentum in z dir

    # back wall
    if ball.pos.z < -side + ball.radius:
        ball.momentum.z = -ball.momentum.z

    if ball2.pos.z < -side + ball2.radius:
        ball2.momentum.z = -ball2.momentum.z

    # if the balls hit each other
    if ball.pos.x + ball.radius == ball2.pos.x - ball2.radius or ball.pos.x - ball.radius == ball2.pos.x + ball2.radius:
        ball.momentum.x = -ball.momentum.x
        ball2.momentum.x = -ball2.momentum.x

    if ball.pos.y + ball.radius == ball2.pos.y - ball2.radius or ball.pos.y - ball.radius == ball2.pos.y + ball2.radius:
        bal.momentum.z = -ball.momentum.z
        ball2.momentum.z = -ball2.momentum.z

    if ball.pos.z + ball.radius == ball2.pos.z - ball2.radius or ball.pos.z - ball.radius == ball2.pos.z + ball2.radius:
        ball.momentum.z = -ball.momentum.z
        ball2.momentum.z = -ball2.momentum.z

    ball.trail.append(pos=ball.pos)
    ball2.trail.append(pos=ball2.pos)

    t = t + dt



