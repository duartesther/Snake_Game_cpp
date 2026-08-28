FROM ubuntu:22.04

# Evita prompts interativos
ENV DEBIAN_FRONTEND=noninteractive

# Instala dependências
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    liballegro5-dev \
    liballegro-acodec5-dev \
    liballegro-audio5-dev \
    liballegro-image5-dev \
    liballegro-ttf5-dev \
    liballegro-ttf5-dev \
    liballegro-dialog5-dev \
    liballegro-physfs5-dev \
    liballegro-video5-dev \
    xorg \
    libgl1-mesa-glx \
    && rm -rf /var/lib/apt/lists/*

# Define diretório de trabalho
WORKDIR /app

# Copia código e assets
COPY . .

# Cria diretório de build e compila
RUN mkdir build && cd build && \
    cmake .. && \
    make -j4

# Comando padrão - executa a versão gráfica (com X11 forwarding)
CMD ["./build/bin/SnakeGame"]