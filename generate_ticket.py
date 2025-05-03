import sys
import openai
from openai import OpenAI
from reportlab.lib.pagesizes import landscape, A6
from reportlab.lib import colors
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas
from reportlab.graphics.barcode import code128
import os
import tempfile
import webbrowser

# Configure l'encodage stdout pour éviter les erreurs d'affichage (Windows)
if sys.stdout.encoding.lower() != 'utf-8':
    try:
        sys.stdout.reconfigure(encoding='utf-8')
    except AttributeError:
        pass  # Pour les consoles qui ne supportent pas reconfigure

# API OpenAI
#APIkey

# Fonction pour générer le texte du ticket
def generate_ticket_text(titre, date_deb, date_fin, prix, id_espace):
    prompt = (
        f"Rédige un ticket professionnel pour un événement avec les informations suivantes :\n"
        f"Titre : {titre}\n"
        f"Date de début : {date_deb}\n"
        f"Date de fin : {date_fin}\n"
        f"Prix : {prix} Dt\n"
        f"ID de l'espace : {id_espace}\n"
        f"Le texte doit être clair, structuré, et prêt à être imprimé."
    )

    response = client.chat.completions.create(
        model="gpt-4",
        messages=[{"role": "user", "content": prompt}]
    )

    return response.choices[0].message.content

# Fonction de création du ticket PDF
def create_ticket_pdf(ticket_text, output_path, ticket_id="TCK123456789"):
    c = canvas.Canvas(output_path, pagesize=landscape(A6))
    width, height = landscape(A6)

    # Définitions des dimensions
    stub_width = 50 * mm
    info_width = width - stub_width

    # Couleurs
    primary_color = colors.HexColor("#FF6600")
    secondary_color = colors.HexColor("#003366")
    white = colors.white
    black = colors.black

    # Fond orange pour la partie détachable
    c.setFillColor(primary_color)
    c.rect(info_width, 0, stub_width, height, stroke=0, fill=1)

    # Bordure extérieure
    c.setStrokeColor(primary_color)
    c.setLineWidth(2)
    c.rect(5, 5, width - 10, height - 10)

    # Ligne de coupe (détachement)
    c.setStrokeColor(black)
    c.setDash(1, 3)
    c.setLineWidth(1)
    c.line(info_width, 10, info_width, height - 10)
    c.setDash()

    # Partie principale
    c.setFillColor(black)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(15, height - 30, "TICKET ÉVÉNEMENT")

    c.setFont("Helvetica", 10)
    y = height - 50
    for line in ticket_text.strip().split('\n'):
        if y < 20: break
        c.drawString(15, y, line.strip())
        y -= 12

    # Partie détachable
    c.setFillColor(white)
    c.setFont("Helvetica-Bold", 9)
    c.drawCentredString(info_width + stub_width / 2, height - 25, "À DÉTACHER")

    # Code-barres
    barcode = code128.Code128(ticket_id, barHeight=30, barWidth=1.2)
    barcode_x = info_width + (stub_width - 100) / 2
    barcode_y = height / 2 - 15
    barcode.drawOn(c, barcode_x, barcode_y)

    # ID texte
    c.setFont("Helvetica-Bold", 9)
    c.drawCentredString(info_width + stub_width / 2, 15, f"ID: {ticket_id}")

    # Finalisation
    c.showPage()
    c.save()

    # Ouvre le PDF
    webbrowser.open('file://' + os.path.realpath(output_path))

# Lancement principal
if __name__ == "__main__":
    if len(sys.argv) < 6:
        print("Erreur: arguments manquants", file=sys.stderr)
        sys.exit(1)

    _, titre, date_deb, date_fin, prix, id_espace = sys.argv
    ticket_text = generate_ticket_text(titre, date_deb, date_fin, prix, id_espace)

    tmp_pdf = os.path.join(tempfile.gettempdir(), "ticket_design_final.pdf")
    create_ticket_pdf(ticket_text, tmp_pdf)

    print("Ticket généré avec succès :", tmp_pdf)
