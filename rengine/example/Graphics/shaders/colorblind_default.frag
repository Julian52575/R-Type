uniform sampler2D texture; // La texture à afficher

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = color;
}
