import AVFoundation
import CoreGraphics
import CoreImage
import Foundation


class Camera: NSObject, AVCaptureVideoDataOutputSampleBufferDelegate {
    let session = AVCaptureSession()
    private(set) var currentFrame: CGImage?

    private let output = AVCaptureVideoDataOutput()
    private let videoQueue = DispatchQueue(label: "com.eamontracey.cubot.camera")

    override init() {
        super.init()
        configure()
    }

    private func configure() {
        session.beginConfiguration()

        guard let device = AVCaptureDevice.default(for: .video),
              let input = try? AVCaptureDeviceInput(device: device),
              session.canAddInput(input),
              session.canAddOutput(output) else {
            session.commitConfiguration()
            return
        }
        session.addInput(input)
        output.setSampleBufferDelegate(self, queue: videoQueue)
        session.addOutput(output)

        session.commitConfiguration()
    }

    func start() {
        if !session.isRunning {
            session.startRunning()
        }
    }

    func stop() {
        if session.isRunning {
            session.stopRunning()
        }
    }

    func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        guard let pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else { return }
        let ciImage = CIImage(cvPixelBuffer: pixelBuffer)
        if let cgImage = CIContext().createCGImage(ciImage, from: ciImage.extent) {
            currentFrame = cgImage
        }
    }
}
