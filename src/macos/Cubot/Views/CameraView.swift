import AVFoundation
import SwiftUI

struct CameraView: View {
    let session: AVCaptureSession

    var body: some View {
        CameraPreview(session: session)
    }
}

struct CameraPreview: NSViewRepresentable {
    let session: AVCaptureSession

    func makeNSView(context: Context) -> NSView {
        let view = NSView()

        let previewLayer = AVCaptureVideoPreviewLayer(session: session)
        previewLayer.videoGravity = .resizeAspectFill
        previewLayer.frame = view.bounds
        previewLayer.autoresizingMask = [
            .layerWidthSizable, .layerHeightSizable,
        ]
        previewLayer.setAffineTransform(CGAffineTransform(scaleX: -1, y: 1))

        view.wantsLayer = true
        view.layer = CALayer()
        view.layer?.addSublayer(previewLayer)

        return view
    }

    func updateNSView(_ nsView: NSView, context: Context) {}
}
