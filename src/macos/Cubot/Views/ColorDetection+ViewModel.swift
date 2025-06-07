import AVFoundation
import Combine
import CoreGraphics
import Cubot
import Dispatch

extension ColorDetectionView {
    class ViewModel: ObservableObject {
        let camera = Camera()
        let colorDetector = ColorDetector()

        @Published var images: [CGImage?] = [nil, nil, nil, nil, nil, nil]
        @Published var imageIndex = 0
        @Published var facesColors: [[Color?]] = Array(
            repeating: Array(repeating: nil, count: 9), count: 6)

        func startCamera() {
            camera.start()
        }

        func stopCamera() {
            camera.stop()
        }

        func capture() {
            guard let image = camera.currentFrame else { return }

            // Crop the image to the camera view.
            let width = image.width
            let height = image.height
            let size = min(width, height)
            let cropRect = CGRect(
                x: (width - size) / 2, y: (height - size) / 2, width: size,
                height: size)
            guard let image = image.cropping(to: cropRect) else { return }

            // Save the image and dispatch a task to detect the colors.
            images[imageIndex] = image
            let currentIndex = imageIndex
            DispatchQueue.global(qos: .userInitiated).async {
                self.detectColors(image: image, imageIndex: currentIndex)
            }
            imageIndex += 1
        }

        func detectColors(image: CGImage, imageIndex: Int) {
            // Assume that the image is already square.
            let faceletSize = image.width / 3

            var faceColors: [Color] = []
            for row in 0..<3 {
                for column in 0..<3 {
                    let rect = CGRect(
                        x: column * faceletSize, y: row * faceletSize,
                        width: faceletSize, height: faceletSize)
                    guard let facelet = image.cropping(to: rect) else { return }
                    guard let pixelData = facelet.toPixelData() else { return }
                    pixelData.withUnsafeBufferPointer { buffer in
                        let color = colorDetector.DetectColor(
                            buffer.baseAddress, buffer.count)
                        faceColors.append(color)
                    }
                }
            }

            DispatchQueue.main.async {
                self.facesColors[imageIndex] = faceColors
            }
        }
    }
}
