import pygame
import time
from tileC import Tile
from random import randint



class Timer():
    seconds=10

    
    def tick(self):
        time.sleep(1)
        self.seconds-=1
    

class Character(pygame.Rect):

    width, height = 32, 32

    def __init__(self, x, y):

        self.tx, self.ty = None, None
        pygame.Rect.__init__(self, x, y, Character.width, Character.height)

    def __str__(self):
        return str(self.get_number())

    def set_target(self, next_tile):
        if self.tx == None and self.ty == None:
            self.tx = next_tile.x
            self.ty = next_tile.y

    def get_number(self):
        
        return ((self.x / self.width) + Tile.H) + ((self.y / self.height) * Tile.V)

    def get_tile(self):

        return Tile.get_tile(self.get_number())

    def rotate(self, direction, original_img):

        if direction == 'n':
            if self.direction != 'n':
                self.direction = 'n'
                south = pygame.transform.rotate(original_img, 90) # CCW
                self.img = pygame.transform.flip(south, False, True)

        if direction == 's':
            if self.direction != 's':
                self.direction = 's'
                self.img = pygame.transform.rotate(original_img, 90) # CCW

        if direction == 'e':
            if self.direction != 'e':
                self.direction = 'e'
                self.img = pygame.transform.flip(original_img, True, False)

        if direction == 'w':
            if self.direction != 'w':
                self.direction = 'w'
                self.img = original_img

class Zombie(Character):

    List = []
    spawn_tiles = (9,42,91,134,193,219,274)
    original_img = pygame.image.load('images/zombie.png')

    def __init__(self, x, y):

        self.direction = 'w'
        self.img = Zombie.original_img
        Character.__init__(self, x, y)
        Zombie.List.append(self)

    @staticmethod
    def update(screen,survivor):
        for zombie in Zombie.List:
            screen.blit(zombie.img, (zombie.x, zombie.y))
        #if survivor.x % 32 == 0 and survivor.y % 32 == 0:
                #if zombie.x % 32 == 0 and zobmie.y % 32 == 0:
            tn=survivor.get_number()
            N=tn-(Tile.V)
            S=tn+(Tile.V)
            E=tn+(Tile.H)
            W=tn-(Tile.H)

            NSEW=[N,S,E,W,tn]
          #  for n in NSEW:
            #    pygame.draw.rect(screen, [66,134,122],Tile.get_tile(n))

            if zombie.get_number() in NSEW:
                survivor.health-=5
                    
                    
                    


             
            if zombie.tx != None and zombie.ty != None: # Target is set

                X = zombie.x - zombie.tx
                Y = zombie.y - zombie.ty

                vel = 4
                if X < 0: # --->
                    zombie.x += vel
                    zombie.rotate('e', Zombie.original_img)

                elif X > 0: # <----
                    zombie.x -= vel
                    zombie.rotate('w', Zombie.original_img)

                if Y > 0: # up
                    zombie.y -= vel
                    zombie.rotate('n', Zombie.original_img)

                elif Y < 0: # dopwn
                    zombie.y += vel
                    zombie.rotate('s', Zombie.original_img)

                if X == 0 and Y == 0:
                    zombie.tx, zombie.ty = None, None
 
    @staticmethod
    def spawn(total_frames, FPS):
        if total_frames % (FPS & 2) == 0:
            r = randint(0, len(Zombie.spawn_tiles) - 1)
            tile_num = Zombie.spawn_tiles[r]
            spawn_node = Tile.get_tile(tile_num)
            Zombie(spawn_node.x, spawn_node.y)



class Survivor(Character):

    def __init__(self, x, y):
        self.health=1000
        self.direction = 'w'
        self.img = pygame.image.load('images/survivor_w.png')

        Character.__init__(self, x, y)

    def movement(self):

        if self.tx != None and self.ty != None: # Target is set

            X = self.x - self.tx
            Y = self.y - self.ty

            vel = 8

            if X < 0: # --->
                self.x += vel
            elif X > 0: # <----
                self.x -= vel

            if Y > 0: # up
                self.y -= vel
            elif Y < 0: # dopwn
                self.y += vel

            if X == 0 and Y == 0:
                self.tx, self.ty = None, None

    def draw(self, screen):
        screen.blit(self.img, (self.x, self.y))

    def rotate(self, direction):

        path = 'images/survivor_'
        png = '.png'

        if direction == 'n':
            if self.direction != 'n':
                self.direction = 'n'
                self.img = pygame.image.load(path + self.direction + png)

        if direction == 's':
            if self.direction != 's':
                self.direction = 's'
                self.img = pygame.image.load(path + self.direction + png)

        if direction == 'e':
            if self.direction != 'e':
                self.direction = 'e'
                self.img = pygame.image.load(path + self.direction + png)

        if direction == 'w':
            if self.direction != 'w':
                self.direction = 'w'
                self.img = pygame.image.load(path + self.direction + png)

class Coin(Character,Timer):

    List = []
    spawn_tiles = (9,42,91,134,193,219,274)
    original_img = pygame.image.load('images/coin.png')
    
    
    def __init__(self, x, y):

        self.direction = 'w'
        self.score=0
        self.img = Coin.original_img
        Coin.List.append(self)
     #   original_img = pygame.image.load('images/coin.png')

        Character.__init__(self, x, y)

    @staticmethod
    def update(screen,survivor):
        for coin in Coin.List:
            screen.blit(coin.img, (coin.x, coin.y))
        #if survivor.x % 32 == 0 and survivor.y % 32 == 0:
                #if zombie.x % 32 == 0 and zobmie.y % 32 == 0:
            tnt=survivor.get_number()
            N=tnt-(Tile.V)
            S=tnt+(Tile.V)
            E=tnt+(Tile.H)
            W=tnt-(Tile.H)

            NSEW=[N,S,E,W,tnt]
          #  for n in NSEW:
            #    pygame.draw.rect(screen, [66,134,122],Tile.get_tile(n))

            if coin.get_number()==tnt:
                coin.score+=1
                Timer.seconds+=5
                Coin.List.remove(coin)

    @staticmethod
    def spawn(total_frames, FPS):
        if total_frames % (FPS * 3) == 0:
            r = randint(0, len(Coin.spawn_tiles) - 1)
            tile_num = Coin.spawn_tiles[r]
            spawn_node = Tile.get_tile(tile_num)
            Coin(spawn_node.x, spawn_node.y)

            
                

