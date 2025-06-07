import CoreGraphics

import Cubot

extension CGImage {
    // Convert the CGImage to an array of RGB pixel data normalized to the range
    // [0, 1].
    func toPixelData() -> [CFloat]? {
        // Extract the raw pixel data. For whatever reason, space must be
        // reserved for the alpha channel, but we ignore alpha values.
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        var pixelData = [UInt8](repeating: 0, count: height * width * 4)
        guard
            let context = CGContext(
                data: &pixelData,
                width: width,
                height: height,
                bitsPerComponent: 8,
                bytesPerRow: width * 4,
                space: colorSpace,
                bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue
            )
        else {
            return nil
        }
        context.draw(self, in: CGRect(x: 0, y: 0, width: width, height: height))

        // Convert the data from RGBA [0, 255] to RGB [0, 1].
        return stride(from: 0, to: pixelData.count, by: 4).flatMap { i in
            (0..<3).map { j in CFloat(pixelData[i + j]) / 255.0 }
        }
    }
}
