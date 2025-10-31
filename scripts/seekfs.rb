class Seekfs < Formula
  desc "Advanced file search utility"
  homepage "https://github.com/maxgog/SeekFS"
  url "https://github.com/yourusername/SeekFS/archive/v1.0.0.tar.gz"
  sha256 "AUTO_GENERATED_SHA256"
  license "GPL-3.0 license"

  depends_on "cmake" => :build

  def install
    system "cmake", "-B", "build", "-DCMAKE_BUILD_TYPE=Release", *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    system "#{bin}/SeekFS", "--help"
  end
end
