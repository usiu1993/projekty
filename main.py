import pygame, sys, Funk
from threading import Thread
from tileC import Tile
from object_classes import *
from interaction import interaction
from A_Star import A_Star
from time import *

pygame.init()
pygame.font.init()

screen = pygame.display.set_mode((704, 448)) # 32, 32


for y in range(0, screen.get_height(), 32):
    for x in range(0, screen.get_width(), 32):
        if Tile.total_tiles in Tile.invalids:
            Tile(x, y, 'solid')
        else:
            Tile(x, y, 'empty')

clock = pygame.time.Clock()
FPS = 20
total_frames = 0

 
dungeon = pygame.image.load('images/tlo.png')
survivor = Survivor(50 * 2, 50 * 4)
#coin=Coin(100*2,50*3)
Zombie(150 * 2, 60 * 4)
Zombie(32 * 2, 32 * 4)
timer=Timer()
    
while True:

    screen.blit(dungeon, (0,0) )
    if total_frames % 6*FPS  == 0:
        print len(Zombie.List)
    #Zombie.spawn(5, FPS)
       
    #Zombie.movement()
    #zombie2.movement()
    t=Thread(target=timer.tick())
    t.start()
    Coin.spawn(total_frames,FPS)
    A_Star(screen, survivor, total_frames, FPS)
    interaction(screen, survivor)
    Tile.draw_tiles(screen)
    survivor.draw(screen)
    Zombie.update(screen,survivor)
    Coin.update(screen,survivor)
    Funk.text_to_screen(screen,survivor.health,0,0)
   
    #Zombie.draw_zombies(screen)
    
    #zombie2.draw_zombies(screen)

    pygame.display.flip()
    clock.tick(FPS)
    total_frames += 1

    if survivor.health ==0:
        sleep(3)
        screen.blit(pygame.image.load('images/dead.jpg'),(0,0))
        pygame.display.update()
        break
sleep(3)
