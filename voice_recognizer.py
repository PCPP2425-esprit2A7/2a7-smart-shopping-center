import speech_recognition as sr

recognizer = sr.Recognizer()

with sr.Microphone() as source:
    recognizer.adjust_for_ambient_noise(source)
    audio = recognizer.listen(source)

try:
    text = recognizer.recognize_google(audio, language="fr-FR")
    print(text, end="")  # ✅ NE PAS AJOUTER de retour à la ligne
except Exception as e:
    print("", end="")  # ✅ Qt doit lire une chaîne vide proprement en cas d'erreur
