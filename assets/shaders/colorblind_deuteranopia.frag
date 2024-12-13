uniform sampler2D texture; // La texture à afficher

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Transformation pour la deutéranopie
    float r = 0.625 * color.r + 0.375 * color.g;
    float g = 0.7 * color.g + 0.3 * color.r;
    float b = color.b;

    gl_FragColor = vec4(r, g, b, color.a);
}