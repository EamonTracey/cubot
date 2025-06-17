import Cubot
import SwiftUI

struct CubePickerView: View {
    @Binding var cube: cubot.Cube?
    @StateObject private var viewModel = ViewModel()

    var body: some View {
        VStack {
            CameraView(session: viewModel.camera.session)
                .aspectRatio(1.0, contentMode: .fit)
                .padding()
                .onAppear { viewModel.startCamera() }
                .onDisappear { viewModel.stopCamera() }
            HStack {
                ForEach(0..<6) { i in
                    VStack {
                        if let image = viewModel.images[i] {
                            Image(decorative: image, scale: 1.0)
                                .resizable()
                                .aspectRatio(contentMode: .fit)
                                .clipped()
                                .clipShape(RoundedRectangle(cornerRadius: 5))
                        } else {
                            RoundedRectangle(cornerRadius: 5)
                                .stroke(
                                    style: StrokeStyle(lineWidth: 1))
                                .aspectRatio(contentMode: .fit)
                                .foregroundColor(
                                    i == viewModel.imageIndex ? .white : .gray
                                )
                        }
                        FaceView(colors: $viewModel.facesColors[i])
                            .aspectRatio(1.0, contentMode: .fit)
                    }
                }
            }
            Button("Capture") {
                viewModel.capture()
            }.disabled(viewModel.imageIndex >= viewModel.images.endIndex)
                .onChange(of: viewModel.facesColors) {
                    _ in cube = viewModel.colorsToValidCube()
                }
        }
    }
}
