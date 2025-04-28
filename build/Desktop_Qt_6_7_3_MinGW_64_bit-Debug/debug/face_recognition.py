import cv2
import sys
import numpy as np
import functools
print("Début du script Python...")  # Pour vérifier si le script commence bien.
sys.stdout.flush()  # Assure-toi que la sortie est bien vidée.
# Forcer l'affichage immédiat
print = functools.partial(print, flush=True)

def detect_and_draw_landmarks(image_path):
    # Charger l'image
    img = cv2.imread(image_path)
    if img is None:
        print(f"Erreur: Impossible de charger l'image: {image_path}")
        return False

    print(f"Image chargée avec succès. Taille: {img.shape}")
    
    # Convertir en niveaux de gris
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # Charger le classificateur Haar Cascade avec un chemin ABSOLU
    cascade_path = "C:/Users/Yasmine/Desktop/testtttfinal/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/haarcascade_frontalface_default.xml"
    face_cascade = cv2.CascadeClassifier(cascade_path)
    if face_cascade.empty():
        print(f"Erreur: Impossible de charger le fichier cascade: {cascade_path}")
        return False
    
    # Détecter les visages
    faces = face_cascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=3,
        minSize=(30, 30)
    )
    
    print(f"Nombre de visages détectés: {len(faces)}")
    
    if len(faces) == 0:
        print("NO_FACE")
        return False
        
    # Dessiner les rectangles sur les visages détectés
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
        print(f"Visage détecté aux coordonnées: x={x}, y={y}, largeur={w}, hauteur={h}")
    
    # Sauvegarder l'image annotée
    output_path = "output_temp.png"
    cv2.imwrite(output_path, img)
    print(f"Image sauvegardée: {output_path}")
    
    return True

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python face_recognition.py <image_path>")
        sys.exit(1)
    
    if detect_and_draw_landmarks(sys.argv[1]):
        print("OK")
    else:
        print("NO_FACE")
sys.exit(0)
