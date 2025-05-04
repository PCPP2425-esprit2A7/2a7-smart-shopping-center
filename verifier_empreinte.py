import cx_Oracle
import sys
import base64

def comparer_empreintes(emp1, emp2):
    # Simple comparaison binaire (à adapter selon ton capteur)
    return emp1 == emp2

def verifier_emp(template_recu):
    try:
        # Connexion à la base Oracle
        dsn = cx_Oracle.makedsn("localhost", 1521, service_name="XE")
        conn = cx_Oracle.connect(user="YOUSSEF", password="esprit18", dsn=dsn)

        cursor = conn.cursor()

        # Récupération de toutes les empreintes et IDs
        cursor.execute("SELECT id, empreinte FROM employe WHERE empreinte IS NOT NULL")

        template_recu_bytes = base64.b64decode(template_recu.encode("utf-8"))

        for emp_id, emp_blob in cursor:
            if emp_blob is not None:
                emp_data = emp_blob.read()
                if comparer_empreintes(emp_data, template_recu_bytes):
                    print(emp_id)
                    return

        print("-1")  # Aucune correspondance

    except Exception as e:
        print(f"Erreur: {e}")
    finally:
        try:
            cursor.close()
            conn.close()
        except:
            pass

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python verifier_empreinte.py <template_base64>")
    else:
        verifier_emp(sys.argv[1])
