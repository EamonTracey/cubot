import AVFoundation
import CoreGraphics
import Cubot
import Foundation

extension ColorDetectionView {
    class ViewModel: ObservableObject {
        let camera = Camera()

        @Published var images: [CGImage?] = [nil, nil, nil, nil, nil, nil]
        @Published var imageIndex = 0

        let colorDetector = ColorDetector()

        func startCamera() {
            camera.start()
        }

        func stopCamera() {
            camera.stop()
        }

        func capture() {
            guard let image = camera.currentFrame else { return }

            let width = image.width
            let height = image.height
            let size = min(width, height)
            let cropRect = CGRect(
                x: (width - size) / 2,
                y: (height - size) / 2,
                width: size,
                height: size
            )

            guard let croppedImage = image.cropping(to: cropRect) else {
                return
            }

            images[imageIndex] = croppedImage
            imageIndex += 1

            DispatchQueue.global(qos: .userInitiated).async {
                if let pixelData = croppedImage.toPixelData() {
                    _ = pixelData.withUnsafeBufferPointer { buffer in
                        print(self.colorDetector.DetectColor(
                            buffer.baseAddress, buffer.count).rawValue)
                    }
                }
            }
        }

    }
}
