import pygame
from object_classes import *
from tileC import Tile

def A_Star(screen, survivor, total_frames, FPS):
    
    half = Tile.width / 2

    N = -22
    S = 22
    E = 1
    W = -1

    NW = -23
    NE = -21
    SE = 23
    SW = 21

    for tile in Tile.List:
        tile.parent = None
        tile.H, tile.G, tile.F = 0,0,0

    def blocky(tiles, diagonals, surrounding_node):
        if surrounding_node.number not in diagonals:
            tiles.append(surrounding_node)
        return tiles

    def get_surrounding_tiles(base_node):
        
        array =(
            (base_node.number + N),
            (base_node.number + NE),
            (base_node.number + E),
            (base_node.number + SE),
            (base_node.number + S),
            (base_node.number + SW),
            (base_node.number + W),
            (base_node.number + NW),
            )

        tiles = []

        onn = base_node.number 
        diagonals = [onn + NE, onn + NW, onn + SE, onn + SW]

        for tile_number in array:

            surrounding_tile = Tile.get_tile(tile_number)
            
            if tile_number not in range(1, Tile.total_tiles + 1):
                continue

            if surrounding_tile.walkable and surrounding_tile not in closed_list:
                # tiles.append(surrounding_tile) # Diagonal movement
                tiles = blocky(tiles, diagonals, surrounding_tile)

        return tiles

    def G(tile):
        
        diff = tile.number - tile.parent.number

        if diff in (N, S, E, W):
            tile.G = tile.parent.G + 10
        elif diff in (NE, NW, SW, SE):
            tile.G = tile.parent.G + 14

    def H():
        for tile in Tile.List:
            tile.H = 10 * (abs(tile.x - survivor.x) + abs(tile.y - survivor.y)) / Tile.width

    def F(tile):
        # F = G + H
        tile.F = tile.G + tile.H

    def swap(tile):
        open_list.remove(tile)
        closed_list.append(tile)

    def get_LFT(): # get Lowest F Value

        F_Values = []
        for tile in open_list:
            F_Values.append(tile.F)

        o = open_list[::-1]

        for tile in o:
            if tile.F == min(F_Values):
                return tile

    def move_to_G_cost(LFT, tile):

        GVal = 0
        diff = LFT.number - tile.number

        if diff in (N, S, E, W):
            GVal = LFT.G + 10
        elif diff in (NE, NW, SE, SW):
            GVal = LFT.G + 14

        return GVal

    def loop():

        LFT = get_LFT() 

        swap(LFT)
        surrounding_nodes = get_surrounding_tiles(LFT)

        for node in surrounding_nodes:

            if node not in open_list:

                open_list.append(node)
                node.parent = LFT

            elif node in open_list:
                
                calculated_G = move_to_G_cost(LFT, node)
                if calculated_G < node.G:

                    node.parent = LFT
                    G(node)
                    F(node)

        if open_list == [] or survivor.get_tile() in closed_list:
            return

        for node in open_list:
            G(node)
            F(node)

            # pygame.draw.line(screen, [255, 0, 0],
            # [node.parent.x + half, node.parent.y + half],
            # [node.x + half, node.y + half] )

        loop()

        

    for zombie in Zombie.List:

        if zombie.tx != None or zombie.ty != None:
            continue

        open_list = []
        closed_list = []

        zombie_tile = zombie.get_tile()
        open_list.append(zombie_tile)

        surrounding_nodes = get_surrounding_tiles(zombie_tile)

        for node in surrounding_nodes:
            node.parent = zombie_tile
            open_list.append(node)      

        swap(zombie_tile)

        H()

        for node in surrounding_nodes:
            G(node)
            F(node) 

        loop()

        return_tiles = []

        parent = survivor.get_tile()

        while True:

            return_tiles.append(parent)

            parent = parent.parent

            if parent == None:
                break

            if parent.number == zombie.get_number():
                break

        for tile in return_tiles:
            pygame.draw.circle(screen, [34, 95, 200],
            [tile.x + half - 2, tile.y + half - 2], 5 )

        if len(return_tiles) > 1:
            next_tile = return_tiles[-1]
            zombie.set_target(next_tile)
