uniform vec2 mouse;       // Position de la souris
uniform vec2 resolution;  // Résolution de la fenêtre
uniform float radius;     // Rayon de l'éclairage
uniform float intensity;  // Intensité maximale de la lumière
uniform sampler2D texture; // Texture de l'arrière-plan (image de la scène)

void main() {
    // Coordonnées normalisées
    vec2 uv = gl_FragCoord.xy / resolution;

    // Convertir la position de la souris en coordonnées normalisées
    vec2 mouseNorm = mouse / resolution;

    // Calculer la distance entre chaque pixel et la souris
    float dist = distance(uv, mouseNorm);

    // Calculer l'intensité de la lumière en fonction de la distance
    // smoothstep crée une transition douce en fonction de la distance
    float light = smoothstep(radius, 0.0, dist) * intensity;

    // Récupérer la couleur originale du pixel de la texture
    vec3 originalColor = texture2D(texture, uv).rgb;

    // Appliquer l'effet de lumière en ajoutant de la lumière proportionnelle sans altérer la couleur de base
    vec3 color = originalColor + light * vec3(intensity);

    // Limiter la couleur pour éviter un dépassement de 1.0 (blanc pur)
    color = min(color, vec3(1.0));

    // Affecter la couleur finale avec alpha égal à 1.0 (opaque)
    gl_FragColor = vec4(color, 1.0);
}
