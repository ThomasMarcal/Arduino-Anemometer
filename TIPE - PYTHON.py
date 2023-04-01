## TIPE 2020/21 : MARCAL Thomas

import csv
import numpy as np
import matplotlib.pyplot as plt

## DONNEES (Fiche technique : Vestas V117-3,3 MW® IEC IIA)

Diametre = 117               # diamètre rotor (en m)
S = np.pi*((Diametre/2)**2)  # Surface balayée (en m^2)
PuissanceNominal = 3300      # en kW
VVentMin = 3                 # en m/S
VVentMax = 25                # en m/S
rho = 1.23                   # masse volumique de l'air

## Puissance électrique en sortie de l'éolienne => Pertes de rendement
# Pertes Mécaniques
PPales = 0.67 # Pales : entre 0,2 et 0,85
PBoite = 0.90 # Boite de vitesse : entre 0,7 et 0,98   
# Pertes électriques (supposé car pas de données trouvables)
PGene = 0.90 # Génératrice : entre 0,85 et 0,98 
PTrans = 0.95 # Transformateur : entre 0,92 et 0,98       
PRed = 0.95   # Redresseur : entre 0,9 et 1   
PBat = 0.95   # Batteries : entre 0,7 et 1
PCab = 0.97  # Pertes dans les câbles : entre 0,9 et 0,99
# Total des pertes de rendement: 
PTot = PPales*PBoite*PGene*PTrans*PRed*PBat*PCab   

## Fonction d'extraction des donnees depuis les résultat arduino
def readCSV(csvFile):
    with open(csvFile,newline='') as csvfile:
        l=csvfile.readline()          # lecture premiere ligne
        NbColonne = len(l.split(',')) # Determination nb  colonnes
        liste=[]                      #Initialisation de liste de listes
        for i in range(NbColonne):
            liste.append([])
        filereader = csv.reader(csvfile, delimiter=',')
        # Iteration et remplissage des listes
        for ligne in filereader:
            for colonne in range(NbColonne):
                liste[colonne].append(float(ligne[colonne]))     
        return(liste)
        
# Extraction des données    
[VitVent1,Temps1]=readCSV('20MAI.csv')
[VitVent2,Temps2]=readCSV('21MAI.csv')

def ListeVit(Vit1,Temps1,Vit2,Temps2): # Creation liste de même taille
    V=[]
    T=[]
    if len(Temps1)<len(Temps2):
        for i in range(len(Temps1)):
            V.append(Vit2[i])
            T.append(Temps2[i])
        VitVentThérorique = np.linspace(0.0,100.0,len(Temps1))
        return(Vit1,Temps1,V,T,VitVentThérorique)
    else:
        for i in range(len(Temps2)):
            V.append(Vit1[i])
            T.append(Temps1[i])
        VitVentThérorique = np.linspace(0.0,100.0,len(Temps2))
        return(V,T,Vit2,Temps2,VitVentThérorique)
        
[V1,T1,V2,T2,VitTheorique]=ListeVit(VitVent1,Temps1,VitVent2,Temps2)
VitVENT=np.linspace(0.0,100/3.6,len(T1))

# Conversion en m/s
Vit1 = [i/3.6 for i in V1]
Vit2 = [i/3.6 for i in V2]
VitTheo = [i/3.6 for i in VitTheorique]
'''
## Tracer données brutes => Evoultution de la vitesse du vent au cours du temps
plt.plot(T1,Vit1,label="20 MAI")
plt.plot(T2,Vit2,label="21 MAI")
plt.plot(T1,VitTheo,"r-",label="Vitesse théorique")
plt.ylabel("Vitesse du vent (m/s)")
plt.xlabel("Temps (s)")
plt.legend()
plt.title("Vitesse du vent au cours du temps")
plt.grid(True)
plt.show()
'''

## Vitesse du vent réellement utile au système
def VitVentEolien(VVent):
    for i in range(len(VVent)):
        if ((VVent[i]<VVentMin) or (VVent[i]>VVentMax)):
            VVent[i]=0
    return(VVent)
'''
## Vitesse du vent utilisable par l'éolienne
plt.plot(T1,VitVentEolien(VitTheo),"r-",label="Vitesse théorique")
plt.ylabel("Vitesse du vent (m/s)")
plt.xlabel("Temps (s)")
plt.legend()
plt.title("Vitesse du vent réellement utilisable")
plt.grid(True)
plt.show()
'''

## Puissance théoriquement récupérable (en kW)
def PRecupThéorique(VVent):
    PRecup = []
    for i in range(len(VVent)):
        PRecup.append(0.5*rho*S*(VVent[i]**3)/1000)
    return(PRecup)
    
## Puissance récupérable maximal (Limite de Betz) (en kW)
def PRecupMax(VVent):
    PRecup=[]
    for i in range(len(VVent)):
        PRecup.append((16/27)*PRecupThéorique(VVent)[i])
    return(PRecup)

## Puissance réellement utilisée par l'éolienne
def PuissanceEolienne(VVent):
    PRecupEolienne = PRecupMax(VitVentEolien(VVent))
    for i in range(len(VVent)):
        if PRecupEolienne[i]>PuissanceNominal:
            PRecupEolienne[i]=PuissanceNominal
        if PRecupEolienne[i]<PuissanceNominal:
            PRecupEolienne[i]=PRecupMax(VVent)[i] 
    return(PRecupEolienne)

'''
## Courbe de puissance théorique de l'éolienne Vestas
plt.plot(VitVENT,PuissanceEolienne(VitTheo),"r-",label="Vitesse théorique")
plt.ylabel("Puissance (en kW)")
plt.xlabel("Vitesse du vent (en m/s)")
plt.title("Courbe de puissance théorique de l'éolienne Vestas")
plt.legend()
plt.grid(True)
plt.show()
'''

## Puissance à la sortie de l'éolienne
def PuissanceSortieTurbine(VVent):
    PSortie = PuissanceEolienne(VVent)
    PSortie = [i*PTot for i in PSortie]
    return(PSortie)
'''
plt.plot(VitVENT,PuissanceSortieTurbine(VitTheo),"r-",label="Vitesse théorique")
plt.ylabel("Puissance (en kW)")
plt.xlabel("Vitesse du vent (en m/s)")
plt.title("Courbe de puissance théorique de l'éolienne Vestas")
plt.legend()
plt.grid(True)
plt.show()
'''

## EFFICACITE 

# Vitesse moyenne du vent à Pontoise(95) 
VMoyIDF = 54/3.6 # en m/s
RendMoy = 0.2    # Rendement moyen d'une eolienne

def Rendement(VVent):
    PMoy = sum(PuissanceSortieTurbine(VVent))/len(VVent) # Moyenne pour la puissance
    Rend = PMoy/(0.5*rho*S*(VMoyIDF**3)/1000) # Rendement global des éoliennes
    return(Rend)

def efficacite(VVent):
    if Rendement(VVent)<RendMoy:
        return("L'éolienne n'est pas rentable. L'installation d'un parc n'est donc pas conseillée.")
    else:
        return("L'éolienne est rentable. L'installation d'un parc est conseillée.")
        
print(efficacite(Vit1))
print(efficacite(Vit2))