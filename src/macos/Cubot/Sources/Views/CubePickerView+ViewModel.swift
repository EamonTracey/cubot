import Combine
import CoreGraphics
import Cubot
import CubotBridge
import Dispatch

extension CubePickerView {
    class ViewModel: ObservableObject {
        let camera = Camera()

        @Published var selectedColor: cubot.Color? = nil
        @Published var selectedFace: cubot.Cube.Facelet = .up
        @Published var scramble: String = ""
        @Published var showCamera = false
        @Published var isHoveringResetScramble = false
        @Published var facesColors: [[cubot.Color?]] = Array(
            repeating: Array(repeating: nil, count: 9), count: 6)

        var isScrambleValid: Bool {
            IsValidAlgorithm(std.string(scramble))
        }

        func startCamera() {
            camera.start()
        }

        func stopCamera() {
            camera.stop()
        }

        func capture() {
            guard let image = camera.currentFrame else { return }

            // Dispatch a task to detect the colors.
            let currentFace = selectedFace
            DispatchQueue.global(qos: .userInitiated).async {
                self.detectColors(image: image, face: currentFace)
            }
        }

        func detectColors(image: CGImage, face: cubot.Cube.Facelet) {
            // Crop the image to the camera view.
            let width = image.width
            let height = image.height
            let size = min(width, height)
            let cropRect = CGRect(
                x: (width - size) / 2, y: (height - size) / 2, width: size,
                height: size)
            guard let image = image.cropping(to: cropRect) else { return }

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
                        let color = cubot.ColorDetector.shared.DetectColor(
                            buffer.baseAddress, buffer.count)
                        faceColors.append(color)
                    }
                }
            }

            DispatchQueue.main.async {
                self.facesColors[Int(face.rawValue)] = faceColors
            }
        }

        func generateCube() -> cubot.Cube? {
            // The centers must be non-nil.
            for faceColors in facesColors {
                guard faceColors[4] != nil else { return nil }
            }

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

            let cube = cubot.Cube(facelets)
            guard cube.IsValid() else { return nil }
            return cube
        }

        func resetToClear() {
            facesColors = Array(
                repeating: Array(repeating: nil, count: 9), count: 6)
        }

        func resetToSolved() {
            facesColors = [
                Array(repeating: .white, count: 9),
                Array(repeating: .yellow, count: 9),
                Array(repeating: .red, count: 9),
                Array(repeating: .orange, count: 9),
                Array(repeating: .green, count: 9),
                Array(repeating: .blue, count: 9),
            ]
        }

        func resetToScramble() {
            var cube = cubot.Cube()
            cube.Execute(cubot.Algorithm(std.string(scramble)))

            let facelets = cube.ToFacelets()
            for i in 0..<6 {
                for j in 0..<9 {
                    let facelet = facelets[i * 9 + j]
                    facesColors[i][j] = cubot.Color(rawValue: facelet.rawValue)
                }
            }
        }
    }
}
