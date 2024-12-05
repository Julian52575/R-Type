uniform sampler2D texture; // La texture à afficher

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Transformation pour la protanopie
    float r = 0.56667 * color.r + 0.43333 * color.g;
    float g = 0.55833 * color.r + 0.44167 * color.g;
    float b = 0.24167 * color.g + 0.75833 * color.b;

    gl_FragColor = vec4(r, g, b, color.a);
}
