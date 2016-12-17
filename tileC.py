import pygame, Funk

class Tile(pygame.Rect):

    List = []
    width, height = 32, 32
    total_tiles = 1
    H, V = 1, 22

    invalids = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
    22,23,44,
    45,55,66,
    67,68,69,70,71,72,73,77,82,88,
    89,94,98,99,100,101,104,110,111,121,126,127,132,
    133,143,149,150,151,152,153,154,
    155,176,
    177,182,196,197,198,
    199,204,220,
    221,222,223,224,225,226,233,244,242,
    243,253,254,255,261,264,
    265,257,275,282,283,286,
    287,288,289,290,291,292,293,294,295,296,297,298,299,
    300,301,302,303,304,305,306,307,308]


    def __init__(self, x, y, Type):

        self.parent = None
        self.H, self.G, self.F = 0,0,0

        self.type = Type
        self.number = Tile.total_tiles
        Tile.total_tiles += 1

        if Type == 'empty':
            self.walkable = True
        else:
            self.walkable = False

        pygame.Rect.__init__(self, (x, y) , (Tile.width, Tile.height) )

        Tile.List.append(self)

    @staticmethod
    def get_tile(number):
        for tile in Tile.List:
            if tile.number == number:
                return tile

    @staticmethod
    def draw_tiles(screen):
        half = Tile.width / 2

        for tile in Tile.List:

            pass

            if not(tile.type == 'empty'):
                pygame.draw.rect(screen, [40, 40, 40], tile )
'''
            if tile.G != 0:
               Funk.text_to_screen(screen, tile.G, tile.x, tile.y + half, color = [120, 157, 40])
            if tile.H != 0:
               Funk.text_to_screen(screen, tile.H, tile.x + half, tile.y + half, color = [20 , 67, 150])
            if tile.F != 0:
               Funk.text_to_screen(screen, tile.F, tile.x + half, tile.y, color = [56, 177, 177])

            Funk.text_to_screen(screen, tile.number, tile.x, tile.y)
'''
