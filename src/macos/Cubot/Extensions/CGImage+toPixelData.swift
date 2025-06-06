import CoreGraphics
import Cubot

extension CGImage {
    // Convert the CGImage to a tensor of shape (h, w, 3) that RGB values of
    // each pixel normalized to the range [0,1].????? // TODO: fix
    func toPixelData() -> [Float]? {
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
        return pixelData.map { Float($0) / 255.0 }
    }
}
