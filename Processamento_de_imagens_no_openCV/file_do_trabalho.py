'''
precisa pelo menos tirar uma ft e poder gravar em memória - tanto faz como fizer, a gravação pode fechar a aplicação ou não

os filtros podem ser por atalhos de teclado mesmo, assim como os stickers - pode ser interessante ter uma legenda pra tudo

dá pra deixar os stickers salvos em memória e aplicá-los apenas na hora de salvar - isso Evita o sticker ter o filtro de imagem junto
não é um problema - a sora disse q pode aplicar o filtro em cima do sticker mesmo

cuidar pra ter uma cópia da imagem original
aplicar os filtros em cima da imagem original sempre

precisa descobrir como q faz um layout com mais de uma imagem

precisa aplicar os efeitos em cima da camera

o trabalho precisa ter a oportunidade de ligar a camera e colocar uma imagem e editar

tem uma biblioteca (tkinter - python) pra criar uma interface pra selecionar arquivo

alguns filtros precisam ser possível aplicar um em cima do outro (tipo inverter imagem e escala de cinza)
'''

import numpy as np
import cv2 as cv
import os
import sys

def mouse_click(event, x, y, flags, param):
    print("mouse_click")
      
    # to check if left mouse 
    # button was clicked
    if event == cv.EVENT_LBUTTONDOWN:
          
        # font for left click event
        font = cv.FONT_HERSHEY_TRIPLEX
        LB = 'Left Button'
          
        # display that left button 
        # was clicked.
        cv.putText(imgAtual, LB, (x, y), 
                    font, 1, 
                    (255, 255, 0), 
                    2) 
        #return imagem
        cv.imshow('Display window', imgAtual)
        print("mouse")
          
          
    # to check if right mouse 
    # button was clicked
    if event == cv.EVENT_RBUTTONDOWN:
           
        # font for right click event
        font = cv.FONT_HERSHEY_SCRIPT_SIMPLEX
        RB = 'Right Button'
          
        # display that right button 
        # was clicked.
        cv.putText(img, RB, (x, y),
                    font, 1, 
                    (0, 255, 255),
                    2)
        cv.imshow('image', img)


def applySticker(background, foreground, pos_x=None, pos_y=None):
    """
    Cola um sticker (foreground) com canal alpha em um fundo (background),
    ajustando posição pelo centro e cortando se ultrapassar as bordas.

    Parameters:
        background: numpy.ndarray
            Imagem de fundo (BGR).
        foreground: numpy.ndarray
            Imagem do sticker (RGBA, com canal alpha).
        pos_x: int
            Posição X do centro do sticker no fundo.
        pos_y: int
            Posição Y do centro do sticker no fundo.

    Returns:
        numpy.ndarray
            Imagem final com o sticker aplicado.
    """
    # Converter o sticker para BGR
    sticker = cv.cvtColor(foreground, cv.COLOR_RGBA2BGR)

    # Separar canais do foreground (com alpha)
    b, g, r, a = cv.split(foreground)

    # Dimensões das imagens
    f_rows, f_cols, _ = foreground.shape
    b_rows, b_cols, _ = background.shape

    # Ajustar pos_x e pos_y para serem o centro background
    if pos_x is None:
        pos_x = b_cols // 2
    if pos_y is None:
        pos_y = b_rows // 2

    # Coordenadas do sticker ajustadas para o centro
    x_start = pos_x - f_cols // 2
    y_start = pos_y - f_rows // 2

    # Calcula os cortes para evitar extrapolação das bordas
    bg_x_start = max(0, x_start)
    bg_y_start = max(0, y_start)
    bg_x_end = min(b_cols, x_start + f_cols)
    bg_y_end = min(b_rows, y_start + f_rows)

    fg_x_start = max(0, -x_start)
    fg_y_start = max(0, -y_start)
    fg_x_end = fg_x_start + (bg_x_end - bg_x_start)
    fg_y_end = fg_y_start + (bg_y_end - bg_y_start)

    # Recorta as regiões de sobreposição
    sticker = sticker[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask = a[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask_inv = cv.bitwise_not(mask)
    roi = background[bg_y_start:bg_y_end, bg_x_start:bg_x_end]

    # Combinar as imagens usando máscaras
    img_bg = cv.bitwise_and(roi, roi, mask=mask_inv)
    img_fg = cv.bitwise_and(sticker, sticker, mask=mask)
    res = cv.add(img_bg, img_fg)

    # Atualizar o fundo com o resultado
    background[bg_y_start:bg_y_end, bg_x_start:bg_x_end] = res

    return background

print("Você deseja utilizar a câmera ou utilizar uma imagem para a edição?")

# todo -> fazer um loop pra evitar inputs errados no 'choice'
choice = input("[Digite 'C' para câmera e 'F' para foto]")
if choice == 'C':
    kernel_size = (5,5)
    cap = cv.VideoCapture(0)

    h = '1'
    f = False
    rec = False
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break

        k = cv.waitKey(1)

        if k == ord("q") or h == ("q"): #filtro sépia
            h = "q"
            sepia_kernel = np.array([[0.272, 0.534, 0.131],
                                     [0.349, 0.686, 0.168],
                                     [0.393, 0.769, 0.189]])
            frameSepia = cv.transform(frame, sepia_kernel)
            frame = frameSepia

        if k == ord("w") or h == ("w"): #gray scale básico
            frameGray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
            h = "w"
            frame = frameGray
        
        if k == ord("e") or h == ("e"): #imagem gaussianBlur
            frameGauss = cv.GaussianBlur(frame, kernel_size, 0)
            h = "e"
            frame = frameGauss

        if k == ord("r") or h == ("r"): #filtro sharpen
            h = "r"
            sharpen_kernel = np.array([[-1, -1, -1],
                                       [-1,  9, -1],
                                       [-1, -1, -1]])
            frameSharpen = cv.filter2D(frame, -1, sharpen_kernel)
            frame = frameSharpen

        if k == ord("t") or h == ("t"): #filtro sobel
            h = "t"
            sobel_kernel = np.array([[-2, -1, 0],
                                     [-1,  1, 1],
                                     [ 0,  1, 2]])
            frameSobel = cv.filter2D(frame, -1, sobel_kernel)
            frame = frameSobel

        if k == ord("y") or h == ("y"): #filtro negativo
            h = "y"
            frameNegative = 255 - frame
            frame = frameNegative

        if k == ord("u") or h == ("u"): #filtro high-pass
            h = "u"
            blurGauss = cv.GaussianBlur(frame, kernel_size, 0)
            apoio = frame.copy()
            frame = apoio - blurGauss

        if k == ord("i") or h == ("i"): #só as bordas
            h = "i"
            kernel_bordas = np.matrix("-1 -1 -1; -1 8 -1; -1 -1 -1", dtype=np.float32)
            final = cv.filter2D(frame, -1, kernel_bordas)
            frame = final

        if k == ord("p") or h == ("p"): #fundo de chromakey - seria legal implementar alguma coisa de deixasse colocar uma imagem no fundo
            # talvez com um blur por média fique menos esquisito
            h = "p"
            match_value = np.array([30, 30, 30], dtype=np.uint8)
            mask = np.all(frame < match_value, axis=-1)
            final = np.where(mask[..., np.newaxis], np.array([0, 0, 255], dtype=np.uint8), frame)
            frame = final

        if k == ord("a") or h == ("a"):
            h = "a"
            kernel_prewit = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], dtype=np.float32)
            framePrewit = cv.filter2D(frame, -1, kernel_prewit)
            frame = framePrewit

        if k == ord("d") or f:
            if f and k == ord("d"): f = False 
            else: f = True
            flipFrame = cv.flip(frame, 0)
            frame = flipFrame

        if k == ord("o") or h == ("o"): #retornar à imagem original
            h = "o"
    
        cv.imshow('frame', frame)
        if k == ord('x'):
            out.release()
            break

        if k == ord("s") or rec:
            if not rec: 
                rec = True
                vid_nome = str(input("Como deseja chamar o vídeo: ") + ".avi")
                fourcc = cv.VideoWriter_fourcc(*'XVID')
                out = cv.VideoWriter(vid_nome, fourcc, 20.0, (640,  480))
            elif rec and k == ord("s"):
                rec = False
                out.release()
                print("Gravação interrompida")

        if rec:
            out.write(frame)
            

        if cv.waitKey(1) & 0xFF == ord('z'):
            nome_imagem_vid = str(input("Como deseja chamar o print: ") + ".png")
            cv.imwrite(nome_imagem_vid, frame)
    
    # Release everything if job is finished
    cap.release()
    cv.destroyAllWindows()



elif choice == 'F':
    print('Coloque o caminho do arquivo de imagem que você deseja manipular.')
    print('O caminho do aquivo atual está aqui: ')
    dir_path = os.path.dirname(os.path.realpath('file_do_trabalho.py'))
    print(dir_path)
    imagem = input()
    img = cv.imread(imagem) #original

    if img is None:
        sys.exit("Could not read the image.")

    cv.imshow("Display window", img)
    global imgAtual
    imgAtual = img
    while True:
        cv.imshow("Display window", imgAtual)
        k = cv.waitKey(0)

        if k == ord("q"): #filtro sépia
            sepia_kernel = np.array([[0.272, 0.534, 0.131],
                                     [0.349, 0.686, 0.168],
                                     [0.393, 0.769, 0.189]])
            imgSepia = cv.transform(img, sepia_kernel)
            imgAtual = imgSepia

        if k == ord("w"): #gray scale básico
            imgGray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
            imgAtual = imgGray

        if k == ord("o"): #retornar à imagem original
            imgAtual = img

        if k == ord("e"): #imagem binarizada
            imgGray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
            imgBin = imgGray.copy()
            fator = 150
            for i in range(imgGray.shape[0]): #percorre linhas
                for j in range(imgGray.shape[1]): #percorre colunas
                    if imgGray[i,j] >= k:
                        imgBin[i,j] = 255
                    else:
                        imgBin[i,j] = 0
            imgAtual = imgBin

        if k == ord("r"): #imagem invertida
            imgInverted = img.copy()
            apoio = img.copy()
            for i in range(apoio.shape[0]): #percorre linhas
                for j in range(apoio.shape[1]): #percorre colunas
                    # Inversão
                    imgInverted[i,j,0] = apoio[i,j,0] ^ 255 # canal B
                    imgInverted[i,j,1] = apoio[i,j,1] ^ 255 # canal G
                    imgInverted[i,j,2] = apoio[i,j,2] ^ 255 # canal R
            
            imgAtual = imgInverted

        if k == ord("t"): #colorização
            imgColored = img.copy()
            apoio = img.copy()
            mColor = [255, 0, 255] #cor colorizadora
            for i in range(apoio.shape[0]): #percorre linhas
                for j in range(apoio.shape[1]): #percorre colunas
                    # Colorização
                    imgColored[i,j,0] = apoio[i,j,0] | mColor[0] # canal B!!!!!!!!!!!!!!!!
                    imgColored[i,j,1] = apoio[i,j,1] | mColor[1] # canal G
                    imgColored[i,j,2] = apoio[i,j,2] | mColor[2] # canal R!!!!

            imgAtual = imgColored

        if k == ord("y"): #Deixar a imagem contornada
            imgContornada = img.copy()
            kernel = np.matrix("-1 -1 -1; -1 8 -1; -1 -1 -1", dtype=np.float32)
            final = cv.filter2D(imgContornada, -1, kernel)
            imgAtual = final

        if k == ord("u"): #aplicando média
            median = cv.medianBlur(img,13)
            imgAtual = median

        if k == ord("i"): #blur
            imgBlur = cv.blur(img, ksize=(13,13))
            imgAtual = imgBlur

        if k == ord("p"):
            kernel_prewit = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], dtype=np.float32)
            imgPrewit = cv.filter2D(img, -1, kernel_prewit)
            imgAtual = imgPrewit

        if k == ord("a"):
            imgFlipped = cv.flip(imgAtual, 0)
            imgAtual = imgFlipped

        #-------------------------------------------------
        #Stickers
        #if k == ord("d"):
        #cv.setMouseCallback('Display window', mouse_click)
        '''
        if cv.EVENT_LBUTTONDOWN: 
            print("mouse")
            cv.setMouseCallback("Display window", mouse_click)
            cv.destroyAllWindows()
            break
        '''
        if k == ord("d"):
            print("sus")


        # posso fazer pro canal azul, vermelho e verde
        # ainda seria interessante colocar uma colorização
        # preciso dar um jeito de manter a imagem invertida nos outros como opção -> pra pra dar uma opção pra tds de manter a imagem atual pra fzr a mudança

        if k == ord("s"):
            nome_imagem = str(input("Como deseja chamar o print: ") + ".png")
            cv.imwrite(nome_imagem, imgAtual)

        if k == ord("x"):
            #cv.imwrite("starry_night.png", img)
            cv.destroyAllWindows()
            break
    

# começo do código



#--------------------------------------------------------------
'''
import numpy as np
import cv2 as cv

def applySticker(background, foreground, pos_x=None, pos_y=None):
    """
    Cola um sticker (foreground) com canal alpha em um fundo (background),
    ajustando posição pelo centro e cortando se ultrapassar as bordas.

    Parameters:
        background: numpy.ndarray
            Imagem de fundo (BGR).
        foreground: numpy.ndarray
            Imagem do sticker (RGBA, com canal alpha).
        pos_x: int
            Posição X do centro do sticker no fundo.
        pos_y: int
            Posição Y do centro do sticker no fundo.

    Returns:
        numpy.ndarray
            Imagem final com o sticker aplicado.
    """
    # Converter o sticker para BGR
    sticker = cv.cvtColor(foreground, cv.COLOR_RGBA2BGR)

    # Separar canais do foreground (com alpha)
    b, g, r, a = cv.split(foreground)

    # Dimensões das imagens
    f_rows, f_cols, _ = foreground.shape
    b_rows, b_cols, _ = background.shape

    # Ajustar pos_x e pos_y para serem o centro background
    if pos_x is None:
        pos_x = b_cols // 2
    if pos_y is None:
        pos_y = b_rows // 2

    # Coordenadas do sticker ajustadas para o centro
    x_start = pos_x - f_cols // 2
    y_start = pos_y - f_rows // 2

    # Calcula os cortes para evitar extrapolação das bordas
    bg_x_start = max(0, x_start)
    bg_y_start = max(0, y_start)
    bg_x_end = min(b_cols, x_start + f_cols)
    bg_y_end = min(b_rows, y_start + f_rows)

    fg_x_start = max(0, -x_start)
    fg_y_start = max(0, -y_start)
    fg_x_end = fg_x_start + (bg_x_end - bg_x_start)
    fg_y_end = fg_y_start + (bg_y_end - bg_y_start)

    # Recorta as regiões de sobreposição
    sticker = sticker[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask = a[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask_inv = cv.bitwise_not(mask)
    roi = background[bg_y_start:bg_y_end, bg_x_start:bg_x_end]

    # Combinar as imagens usando máscaras
    img_bg = cv.bitwise_and(roi, roi, mask=mask_inv)
    img_fg = cv.bitwise_and(sticker, sticker, mask=mask)
    res = cv.add(img_bg, img_fg)

    # Atualizar o fundo com o resultado
    background[bg_y_start:bg_y_end, bg_x_start:bg_x_end] = res

    return background

#-------------------------------------------------------

# Carregar imagens
background = cv.imread('baboon.png')  # Fundo
foreground = cv.imread('eyeglasses.png', cv.IMREAD_UNCHANGED)  # Sticker com canal alpha

# Aplicar sticker no centro da imagem
res = applySticker(background, foreground, 256, 100)

# Exibir resultado
cv.imshow('Resultado', res)
cv.waitKey(0)
cv.destroyAllWindows()

===============================================================================================


import cv2
   
# read image
img = cv2.imread('baboon.png')
  
# show image
cv2.imshow('image', img)
   
#define the events for the
# mouse_click.
def mouse_click(event, x, y, 
                flags, param):
      
    # to check if left mouse 
    # button was clicked
    if event == cv2.EVENT_LBUTTONDOWN:
          
        # font for left click event
        font = cv2.FONT_HERSHEY_TRIPLEX
        LB = 'Left Button'
          
        # display that left button 
        # was clicked.
        cv2.putText(img, LB, (x, y), 
                    font, 1, 
                    (255, 255, 0), 
                    2) 
        cv2.imshow('image', img)
          
          
    # to check if right mouse 
    # button was clicked
    if event == cv2.EVENT_RBUTTONDOWN:
           
        # font for right click event
        font = cv2.FONT_HERSHEY_SCRIPT_SIMPLEX
        RB = 'Right Button'
          
        # display that right button 
        # was clicked.
        cv2.putText(img, RB, (x, y),
                    font, 1, 
                    (0, 255, 255),
                    2)
        cv2.imshow('image', img)
  
cv2.setMouseCallback('image', mouse_click)
   
cv2.waitKey(0)
  
# close all the opened windows.
cv2.destroyAllWindows()
'''