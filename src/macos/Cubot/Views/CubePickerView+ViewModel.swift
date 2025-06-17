import AVFoundation
import Combine
import CoreGraphics
import Cubot
import CubotBridge
import Dispatch

extension CubePickerView {
    class ViewModel: ObservableObject {
        let camera = Camera()
        let colorDetector = cubot.ColorDetector()

        @Published var images: [CGImage?] = [nil, nil, nil, nil, nil, nil]
        @Published var imageIndex = 0
        @Published var facesColors: [[cubot.Color?]] = Array(
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

            var faceColors: [cubot.Color] = []
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

        func colorsToValidCube() -> cubot.Cube? {
            // All the colors must be non-nil.
            guard
                facesColors.allSatisfy({ faceColors in
                    faceColors.allSatisfy { color in
                        color != nil
                    }
                })
            else { return nil }

            // The centers must be distinct colors.
            guard
                Set([
                    facesColors[0][4], facesColors[1][4], facesColors[2][4],
                    facesColors[3][4], facesColors[4][4], facesColors[5][4],
                ]).count == 6
            else { return nil }

            // The centers define the mapping from colors to facelets.
            let faceletMap: [cubot.Color: cubot.Cube.Facelet] = [
                facesColors[0][4]!: .up,
                facesColors[1][4]!: .down,
                facesColors[2][4]!: .right,
                facesColors[3][4]!: .left,
                facesColors[4][4]!: .front,
                facesColors[5][4]!: .back,
            ]

            var facelets = FaceletsArray()
            for (i, faceColors) in facesColors.enumerated() {
                for (j, color) in faceColors.enumerated() {
                    guard let color = color else { return nil }
                    guard let facelet = faceletMap[color] else { return nil }
                    facelets[i * 9 + j] = facelet
                }
            }

            // Require that the cube is valid.
            let cube = cubot.Cube(facelets)

            print("facelets")
            for i in 0..<54 {
                print(facelets[i].rawValue, terminator: " ")
            }

            print("we instantiated a cube!")

            guard cube.IsValid() else {
                print("cube is invalid");
                for i in 0..<12 {
                    print(cube.edges().pointee[i].orientation.rawValue, cube.edges().pointee[i].solved_position.rawValue)
                }
                return nil }

            print("cube is valid!!!")

            return cube
        }
    }
}
