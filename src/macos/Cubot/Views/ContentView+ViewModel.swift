import Cubot
import SwiftUI

extension ContentView {
    class ViewModel: ObservableObject {
        @Published var cube: cubot.Cube? = nil
        @Published var solution: String? = nil

        func solve() {
            if let cube = cube {
                solution = String(
                    cubot.ThistlethwaiteSolver.shared.Solve(cube)
                        .ToStringNotation())
            }
        }
    }
}
