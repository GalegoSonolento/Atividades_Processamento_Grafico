import numpy as np
import cv2 as cv
import os
import sys

def coloca_atalhos(): #coloca a janela com todos os atalhos do sistema
    atalhos=np.zeros((360,600),dtype=np.uint8)
    cv.putText(atalhos,"Para Camera:",(10,20),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"q - filtro sepia",(10,40),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"w - gray scale",(10,60),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"e - blur guassiano",(10,80),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"r - filtro sharpen",(10,100),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"t - filtro sobel",(10,120),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"y - filtro negativo",(10,140),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"u - filtro high-pass",(10,160),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"i - filtro bordas",(10,180),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"p - fundo de chroma key",(10,200),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"a - filtro prewit",(10,220),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"d - virar o frame",(10,240),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"o - original",(10,260),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"x - fecha a janela",(10,280),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"s - salva o vídeo",(10,300),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"1 - coloca figurinhas (para de gravar)",(10,320),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"z - tira print",(10,340),cv.FONT_HERSHEY_PLAIN,1,255)
    #===========================================================================================
    cv.putText(atalhos,"Para Foto:",(350,20),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"q - filtro sepia",(350,40),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"w - gray scale",(350,60),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"o - original",(350,80),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"e - imagem binarizada",(350,100),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"r - imagem invertida",(350,120),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"t - colorização roxa",(350,140),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"y - aplicar contornos",(350,160),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"u - aplicação de média",(350,180),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"i - blur simples",(350,200),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"p - filtro pritwet",(350,220),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"a - virar a imagem",(350,240),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"1 - aplicar figurinhas",(350,260),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"s - salvar imagem",(350,280),cv.FONT_HERSHEY_PLAIN,1,255)
    cv.putText(atalhos,"x - fecha a janela",(350,300),cv.FONT_HERSHEY_PLAIN,1,255)

    cv.imshow("Atalhos",atalhos)
    cv.imwrite("fonte_opencv.jpg",atalhos)


def mouse_click(event, x, y, flags, param):
    global podeClicar
    if podeClicar:
        # to check if left mouse 
        # button was clicked
        if event == cv.EVENT_LBUTTONDOWN:            
            # font for left click event
            print("Escolha uma das cinco opções de stickers:")
            print("1 - Óculos thug life \n2 - Borracha \n3 - LOL kk \n4 - Coração \n5 - Peperoni")
            esc = int(input())

            foreground = 0
            if esc == 1:
                foreground = cv.imread("eyeglasses.png", cv.IMREAD_UNCHANGED)
            elif esc == 2:
                foreground = cv.imread("borracha.png", cv.IMREAD_UNCHANGED)
            elif esc == 3:
                foreground = cv.imread("lol.png", cv.IMREAD_UNCHANGED)
            elif esc == 4:
                foreground = cv.imread("coracao.png", cv.IMREAD_UNCHANGED)
            elif esc == 5:
                foreground = cv.imread("Peperoni.png", cv.IMREAD_UNCHANGED)
            applySticker(imgAtual, foreground, x, y)

            podeClicar = False
    cv.imshow('Display window', imgAtual)        


def applySticker(background, foreground, pos_x=None, pos_y=None):
    # Converter o sticker para BGR
    sticker = cv.cvtColor(foreground, cv.COLOR_RGBA2RGB)

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
global imgAtual
#Definição de qual tipo de editor
choice = input("[Digite 'C' para câmera e 'F' para foto]")
coloca_atalhos()
if choice == 'C':
    global sticker
    sticker = False
    kernel_size = (5,5)
    cap = cv.VideoCapture(0)
    cam = True
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

        if k == ord("p") or h == ("p"): #fundo de chromakey
            h = "p"
            match_value = np.array([30, 30, 30], dtype=np.uint8)
            mask = np.all(frame < match_value, axis=-1)
            final = np.where(mask[..., np.newaxis], np.array([0, 0, 255], dtype=np.uint8), frame)
            frame = final

        if k == ord("a") or h == ("a"): #filtro prewit
            h = "a"
            kernel_prewit = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], dtype=np.float32)
            framePrewit = cv.filter2D(frame, -1, kernel_prewit)
            frame = framePrewit

        if k == ord("d") or f: #inverter o frame
            if f and k == ord("d"): f = False 
            else: f = True
            flipFrame = cv.flip(frame, 0)
            frame = flipFrame

        if k == ord("o") or h == ("o"): #retornar à imagem original
            h = "o"
    
        cv.imshow('frame', frame) #fechar as janelas
        if k == ord('x'):
            out.release()
            break

        if k == ord("s") or rec: #salvar vídeo
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
            

        if k == ord("1"): #colocar as figurinhas
            podeClicar = True
            imgAtual = frame.copy()
            cv.imshow('frame', imgAtual)
            #out.release()
            sticker = True
            
            break

        if cv.waitKey(1) & 0xFF == ord('z'): #tirar uma print do vídeo
            podeClicar = True
            nome_imagem_vid = str(input("Como deseja chamar o print: ") + ".png")
            cv.imwrite(nome_imagem_vid, frame)
    
    # Release everything if job is finished
    cap.release()
    cv.destroyAllWindows()

    # aqui vai pegar caso ord("1") tenha ativação - vai permitir colocar algumas figurinhas na imagem
    if sticker:
        cv.imshow("Display window", imgAtual)
        cv.setMouseCallback("Display window", mouse_click)
        coloca_atalhos()
        while True:
            cv.imshow("Display window", imgAtual)
            k = cv.waitKey(0)

            if k == ord("1"): #colocar as figurinhas
                podeClicar = True

            if k == ord("s"): #salvar a imagem
                nome_imagem = str(input("Como deseja chamar o print: ") + ".png")
                cv.imwrite(nome_imagem, imgAtual)

            if k == ord("x"): #fecha as janelas
                #cv.imwrite("starry_night.png", img)
                cv.destroyAllWindows()
                break
        cv.destroyAllWindows



elif choice == 'F': #escolha das fotos
    print('Coloque o caminho do arquivo de imagem que você deseja manipular.')
    print('O caminho do aquivo atual está aqui: ')
    dir_path = os.path.dirname(os.path.realpath('file_do_trabalho.py'))
    print(dir_path)
    imagem = input() #user precisa colocar o path da imagem em relação ao arquivo
    img = cv.imread(imagem) #original

    if img is None:
        sys.exit("Could not read the image.")

    cv.imshow("Display window", img)
    imgAtual = img
    recupera = img.copy()
    podeClicar = False
    cv.setMouseCallback("Display window", mouse_click)
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
            imgAtual = recupera.copy()

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

        if k == ord("p"): #filtro prewit
            kernel_prewit = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], dtype=np.float32)
            imgPrewit = cv.filter2D(img, -1, kernel_prewit)
            imgAtual = imgPrewit

        if k == ord("a"): #inverter imagem
            imgFlipped = cv.flip(imgAtual, 0)
            imgAtual = imgFlipped

        if k == ord("1"): #colocar figurinha 
            podeClicar = True

        if k == ord("s"): #salvar a imagem
            nome_imagem = str(input("Como deseja chamar a imagem: ") + ".png")
            cv.imwrite(nome_imagem, imgAtual)

        if k == ord("x"): #fecha todas as janelas
            cv.destroyAllWindows()
            break