uniform sampler2D texture; // La texture à afficher

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Transformation pour la tritanopie
    float r = 0.95 * color.r + 0.05 * color.g;
    float g = 0.43333 * color.g + 0.56667 * color.b;
    float b = 0.475 * color.g + 0.525 * color.b;

    gl_FragColor = vec4(r, g, b, color.a);
}
