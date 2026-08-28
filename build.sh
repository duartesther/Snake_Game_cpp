#!/bin/bash

set -e

echo "🐍 Construindo Snake Game..."

# Criar diretório de build
mkdir -p build
cd build

# Configurar com CMake
echo "📦 Configurando projeto..."
cmake ..

# Compilar
echo "🔨 Compilando..."
make -j$(nproc)

echo "✅ Build concluído!"
echo "🎮 Para executar versão gráfica: ./build/bin/SnakeGame"
echo "📟 Para executar versão texto: ./build/bin/SnakeText"