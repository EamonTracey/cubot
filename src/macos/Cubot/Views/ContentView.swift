import Cubot
import SwiftUI

struct ContentView: View {
    @StateObject private var viewModel = ViewModel()

    var body: some View {
        VStack {
            CubePickerView(cube: $viewModel.cube)
            Button("Solve") {
                viewModel.solve()
            }
            if let solution = viewModel.solution {
                Text("Solution: \(solution)")
            }
        }.onAppear {
            cubot.ThistlethwaiteSolver.loadSharedInBackground()
        }
    }
}
