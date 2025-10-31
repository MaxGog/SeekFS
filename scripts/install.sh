set -e

echo "🚀 Installing SeekFS..."

OS=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

case $ARCH in
    x86_64) ARCH="amd64" ;;
    arm64) ARCH="arm64" ;;
    *) ARCH="unknown" ;;
esac

REPO="MaxGog/SeekFS"
LATEST_RELEASE=$(curl -s https://api.github.com/repos/$REPO/releases/latest | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')

echo "📦 Downloading SeekFS $LATEST_RELEASE for $OS-$ARCH"

case $OS in
    linux)
        curl -L "https://github.com/$REPO/releases/download/$LATEST_RELEASE/seekfs-linux-$ARCH.deb" -o seekfs.deb
        sudo dpkg -i seekfs.deb
        rm seekfs.deb
        ;;
    darwin)
        curl -L "https://github.com/$REPO/releases/download/$LATEST_RELEASE/seekfs-macos-$ARCH.tar.gz" -o seekfs.tar.gz
        tar xzf seekfs.tar.gz
        sudo mv seekfs /usr/local/bin/
        rm seekfs.tar.gz
        ;;
    *)
        echo "❌ Unsupported OS: $OS"
        exit 1
        ;;
esac

echo "✅ SeekFS installed successfully!"
echo "💡 Run 'SeekFS --help' to get started"
