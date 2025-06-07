import Cubot
import SwiftUI

struct ColorDetectionView: View {
    @StateObject private var viewModel = ViewModel()

    var body: some View {
        VStack {
            Spacer()
            CameraView(session: viewModel.camera.session)
                .aspectRatio(contentMode: .fit)
                .frame(width: 400, height: 400)
                .onAppear { viewModel.startCamera() }
                .onDisappear { viewModel.stopCamera() }
            Spacer()
            HStack {
                ForEach(0..<6) { i in
                    Spacer()
                    VStack {
                        if let image = viewModel.images[i] {
                            Image(decorative: image, scale: 1.0)
                                .resizable()
                                .aspectRatio(contentMode: .fill)
                                .frame(width: 100, height: 100)
                                .clipped()
                                .clipShape(RoundedRectangle(cornerRadius: 10))
                        } else {
                            RoundedRectangle(cornerRadius: 10)
                                .stroke(
                                    style: StrokeStyle(
                                        lineWidth: 2, dash: [6, 4])
                                )
                                .foregroundColor(
                                    i == viewModel.imageIndex ? .white : .gray
                                )
                                .frame(width: 100, height: 100)
                        }
                        FaceView(colors: $viewModel.facesColors[i])
                            .frame(width: 100, height: 100)
                            .padding(.top)
                    }
                    Spacer()
                }
            }
            Spacer()
            Button("Capture") {
                viewModel.capture()
            }.disabled(viewModel.imageIndex >= viewModel.images.endIndex)
            Spacer()
        }
    }
}
