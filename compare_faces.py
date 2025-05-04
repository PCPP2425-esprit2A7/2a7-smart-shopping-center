import face_recognition
import sys
import os

def compare_faces(image1_path, image2_path, tolerance=0.4):
    try:
        if not os.path.exists(image1_path) or not os.path.exists(image2_path):
            return {"status": "error", "message": "Un des fichiers d'image est introuvable"}

        image1 = face_recognition.load_image_file(image1_path)
        image2 = face_recognition.load_image_file(image2_path)

        face_locations1 = face_recognition.face_locations(image1)
        face_locations2 = face_recognition.face_locations(image2)

        if not face_locations1:
            return {"status": "error", "message": "Aucun visage détecté dans l'image temporaire"}
        if not face_locations2:
            return {"status": "error", "message": "Aucun visage détecté dans l'image stockée"}

        encoding1 = face_recognition.face_encodings(image1, face_locations1)[0]
        encoding2 = face_recognition.face_encodings(image2, face_locations2)[0]

        results = face_recognition.compare_faces([encoding1], encoding2, tolerance)

        return {"status": "success", "match": results[0]}
    except Exception as e:
        return {"status": "error", "message": str(e)}

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("ERROR")
        sys.exit(1)

    result = compare_faces(sys.argv[1], sys.argv[2])

    if result.get("status") == "success":
        if result.get("match"):
            print("OK")
        else:
            print("NO")
    else:
        print("ERROR")
