#version 330 core
out vec4 FragColor;

// C++'tan gelecek olan zaman verisi
uniform float oyunZamani; 

void main()
{
    // Zamanı kullanarak renklerin yanıp sönmesini sağla
    float a = (sin(oyunZamani) / 2.0f) + 0.5f; 
    FragColor = vec4(0.0f, a, 0.0f, 1.0f); // Renkleri ayarla
}