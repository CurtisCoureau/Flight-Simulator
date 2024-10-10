#include <iostream>
#include <cmath>
#include <fstream>

// Classe Airplane pour gérer les données de vol de base
class Airplane {
public:
    double altitude; // Altitude en mètres
    double velocity; // Vitesse en m/s
    double pitch;    // Angle de tangage en degrés (contrôle la montée/descente)
    double thrust;   // Poussée de l'avion (contrôle la vitesse)

    Airplane() : altitude(1000), velocity(0), pitch(0), thrust(0) {}

    // Met à jour la position de l'avion en fonction de la vitesse et de l'angle de tangage
    void update(double deltaTime) {
        // Mise à jour de la vitesse en fonction de la poussée
        velocity += thrust * deltaTime;

        // Application de la gravité pour simuler la chute
        const double gravity = 9.81; // Accélération due à la gravité en m/s²
        altitude -= gravity * deltaTime;

        // Conversion de l'angle de tangage en radians pour le calcul de la montée/descente
        double pitchRadians = pitch * M_PI / 180.0;

        // Mise à jour de l'altitude en fonction de la vitesse et de l'angle de tangage
        altitude += velocity * std::sin(pitchRadians) * deltaTime;

        // Empêcher l'altitude de descendre en dessous de zéro
        if (altitude < 0) {
            altitude = 0;
            std::cout << "L'avion a touché le sol !\n";
        }

        if (velocity > 300) velocity = 300;  // Limite la vitesse maximale à 300 m/s
        if (velocity < 0) velocity = 0;      // Empêche la vitesse de devenir négative
        if (pitch > 45) pitch = 45;          // Limite le tangage à un maximum de 45 degrés
        if (pitch < -45) pitch = -45;        // Limite le tangage à un minimum de -45 degrés
    }

    // Affiche les données de vol actuelles
    void displayStatus() const {
        std::cout << "Altitude: " << altitude << " m, "
                  << "Vitesse: " << velocity << " m/s, "
                  << "Tangage: " << pitch << " deg, "
                  << "Poussée: " << thrust << "\n";
    }
};

// Fonction pour gérer les entrées utilisateur
bool handleInput(Airplane &plane) {
    char input;
    std::cout << "Commandes : 'z' pour augmenter le tangage, 's' pour le réduire\n";
    std::cout << "'e' pour réduire la poussée, 'a' pour l'augmenter, 'q' pour quitter.\n";
    std::cin >> input;

    switch (input) {
        case 'z':
            plane.pitch += 5; // Augmente le tangage de 5 degrés
            break;
        case 's':
            plane.pitch -= 5; // Réduit le tangage de 5 degrés
            break;
        case 'e':
            plane.thrust -= 0.1; // Réduit la poussée
            break;
        case 'a':
            plane.thrust += 0.1; // Augmente la poussée
            break;
        case 'q':
            std::cout << "Simulation terminée.\n";
            return false;  // Arrête la boucle
        default:
            std::cout << "Commande invalide.\n";
            break;
    }
    return true;  // Continue la boucle
}

void logData(const Airplane& plane, std::ofstream& logFile) {
    logFile << plane.altitude << ", " << plane.velocity << ", " << plane.pitch << ", " << plane.thrust << "\n";
    logFile.flush();  // Assure que les données sont immédiatement écrites dans le fichier
}

int main() {
    Airplane plane;
    double deltaTime = 1.0;
    std::ofstream logFile("flight_data.csv");

    // En-tête du fichier CSV
    logFile << "Altitude, Vitesse, Tangage, Poussée\n";

    std::cout << "Bienvenue dans la simulation de vol simplifiée !\n";

    bool running = true;
    while (running) {
        plane.update(deltaTime);
        plane.displayStatus();
        logData(plane, logFile);  // Enregistre les données à chaque mise à jour
        running = handleInput(plane);
    }

    logFile.close();
    return 0;
}
