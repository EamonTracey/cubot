import Cubot
import SwiftUI

extension MainView {
    class ViewModel: ObservableObject {
        @Published var cube: cubot.Cube? = nil
        @Published var solution: String? = nil
        @Published var isHoveringSolve = false

        var isCubeValid: Bool {
            cube?.IsValid() ?? false
        }

        var isCubeSolvable: Bool {
            cube?.IsSolvable() ?? false
        }

        func solve() {
            if let cube = cube {
                solution = String(
                    cubot.TwoPhaseSolver.shared.Solve(cube, -1)
                        .ToStringNotation())
            }
        }
    }
}
