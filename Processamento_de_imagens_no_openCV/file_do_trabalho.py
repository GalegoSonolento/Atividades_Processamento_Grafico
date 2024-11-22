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

print("Você deseja utilizar a câmera ou utilizar uma imagem para a edição?")

# todo -> fazer um loop pra evitar inputs errados no 'choice'
choice = input("[Digite 'C' para câmera e 'F' para foto]")
if choice == 'C':
    cap = cv.VideoCapture(0)

    # Define the codec and create VideoWriter object
    fourcc = cv.VideoWriter_fourcc(*'XVID')
    out = cv.VideoWriter('output.avi', fourcc, 20.0, (640,  480))
    
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
        
        out.write(frame)
    
        cv.imshow('frame', frame)
        if cv.waitKey(1) == ord('q'):
            break
    
    # Release everything if job is finished
    cap.release()
    out.release()
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
    k = cv.waitKey(0)
    
    # Esse comando permite salvar a imagem colocada no OpenCV
    if k == ord("s"):
        cv.imwrite("starry_night.png", img)

# começo do código



#--------------------------------------------------------------