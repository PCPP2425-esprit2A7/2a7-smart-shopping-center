import face_recognition
import sys
import os
import logging
import traceback

# Configure logging
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[logging.FileHandler('face_comparison.log'), logging.StreamHandler()]
)

def validate_image_file(file_path):
    """Validate image file existence, readability, and size"""
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"File not found: {file_path}")
    if not os.access(file_path, os.R_OK):
        raise PermissionError(f"No read permission for file: {file_path}")
    if os.path.getsize(file_path) == 0:
        raise ValueError(f"Empty file: {file_path}")
    if not file_path.lower().endswith(('.png', '.jpg', '.jpeg')):
        raise ValueError("Unsupported file format. Only PNG/JPEG images are supported")

def compare_faces(image1_path, image2_path, tolerance=0.4):
    try:
        # Validate input files
        for path in [image1_path, image2_path]:
            validate_image_file(path)

        # Load images with error handling
        try:
            image1 = face_recognition.load_image_file(image1_path)
            image2 = face_recognition.load_image_file(image2_path)
        except Exception as e:
            raise RuntimeError(f"Failed to load image: {str(e)}")

        # Detect faces with multiple attempts
        face_locations1 = face_recognition.face_locations(image1)
        face_locations2 = face_recognition.face_locations(image2)

        if not face_locations1:
            raise ValueError("No faces detected in image 1")
        if not face_locations2:
            raise ValueError("No faces detected in image 2")
        if len(face_locations1) > 1:
            logging.warning("Multiple faces detected in image 1 - using first face")
        if len(face_locations2) > 1:
            logging.warning("Multiple faces detected in image 2 - using first face")

        # Get encodings with safety checks
        try:
            encoding1 = face_recognition.face_encodings(image1, face_locations1)[0]
            encoding2 = face_recognition.face_encodings(image2, face_locations2)[0]
        except IndexError:
            raise RuntimeError("Face encoding failed - no valid face detected")

        # Perform comparison
        results = face_recognition.compare_faces([encoding1], encoding2, tolerance)
        
        return {
            "status": "success",
            "match": bool(results[0]),
            "confidence": float(1 - face_recognition.face_distance([encoding1], encoding2)[0])
        }

    except Exception as e:
        logging.error(f"Error in face comparison: {str(e)}\n{traceback.format_exc()}")
        return {
            "status": "error",
            "message": str(e),
            "error_type": type(e).__name__
        }

if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            raise ValueError("Requires exactly 2 image paths as arguments")

        result = compare_faces(sys.argv[1], sys.argv[2])

        if result.get("status") == "success":
            print("OK" if result["match"] else "NO")
        else:
            print("ERROR")
            sys.exit(1)

    except Exception as e:
        logging.error(f"Critical error: {str(e)}\n{traceback.format_exc()}")
        print("ERROR")
        sys.exit(2)