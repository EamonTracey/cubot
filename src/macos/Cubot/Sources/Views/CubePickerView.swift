import Cubot
import SwiftUI

struct CubePickerView: View {
    @Binding var cube: cubot.Cube?

    @StateObject private var viewModel = ViewModel()
    @FocusState private var isFocusedScramble

    var body: some View {
        VStack {
            CubotColorPickerView(color: $viewModel.selectedColor)
                .frame(maxWidth: .infinity, maxHeight: 35)
            if viewModel.showCamera {
                VStack {
                    CameraView(session: viewModel.camera.session)
                        .aspectRatio(1.0, contentMode: .fit)
                        .clipped()
                        .onAppear { viewModel.startCamera() }
                        .onDisappear { viewModel.stopCamera() }
                    Button("Capture") { viewModel.capture() }
                }
                .padding()
            } else {
                NetView(facesColors: $viewModel.facesColors)
                    .padding()
            }
            HStack {
                Button(
                    viewModel.showCamera ? "Hide camera" : "Show camera",
                    systemImage: "camera"
                ) { viewModel.showCamera.toggle() }
                Picker("", selection: $viewModel.selectedFace) {
                    ForEach(cubot.Cube.Facelet.allCases, id: \.self) {
                        facelet in
                        Text("\(facelet.string)").tag(facelet)
                    }
                }
                .pickerStyle(SegmentedPickerStyle())
            }
            .padding(.bottom)
            HStack {
                Button("Reset to clear") { viewModel.resetToClear() }
                Button("Reset to solved") { viewModel.resetToSolved() }
                Button("Reset to scramble") { viewModel.resetToScramble() }
                    .disabled(!viewModel.isScrambleValid)
                    .onHover { isHovering in
                        viewModel.isHoveringResetScramble = isHovering
                    }
                    .popover(
                        isPresented: .constant(
                            viewModel.isHoveringResetScramble
                                && !viewModel.isScrambleValid)
                    ) {
                        Text("The scramble is not valid.")
                            .foregroundStyle(.red)
                            .padding()
                    }
                TextField("Scramble", text: $viewModel.scramble)
                    .autocorrectionDisabled()
                    .onSubmit { isFocusedScramble = false }
                    .focused($isFocusedScramble)
            }
        }
        .onChange(of: viewModel.facesColors) { cube = viewModel.generateCube() }
        .environmentObject(viewModel)
    }
}
