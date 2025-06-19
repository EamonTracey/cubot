import Cubot
import SwiftUI

struct MainView: View {
    @StateObject private var viewModel = ViewModel()

    var body: some View {
        VStack {
            CubePickerView(cube: $viewModel.cube)
                .padding()
            Spacer()
            Button {
                viewModel.solve()
            } label: {
                Label("Solve", systemImage: "puzzlepiece")
                    .font(.title.bold())
                    .padding()
            }
            .disabled(!(viewModel.isCubeValid && viewModel.isCubeSolvable))
            .onHover { isHovering in viewModel.isHoveringSolve = isHovering }
            .padding()
            .popover(
                isPresented: .constant(
                    viewModel.isHoveringSolve
                        && !(viewModel.isCubeValid && viewModel.isCubeSolvable))
            ) {
                VStack {
                    if !viewModel.isCubeValid {
                        Text("The cube is not valid.")
                            .foregroundStyle(.red)
                    }
                    if !viewModel.isCubeSolvable {
                        Text("The cube is not solvable.")
                            .foregroundStyle(.red)
                    }
                }
                .padding()
            }
            Text("Solution: \(viewModel.solution ?? "")")
                .font(.title)
            Spacer()

        }
        .padding()
        .onAppear {
            cubot.ColorDetector.loadSharedInBackground()
            cubot.ThistlethwaiteSolver.loadSharedInBackground()
        }

    }
}
