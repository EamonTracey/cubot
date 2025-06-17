import Cubot
import Dispatch

extension cubot.ThistlethwaiteSolver {
    private static var _shared: cubot.ThistlethwaiteSolver?
    private static let lock = DispatchQueue(label: "ThistlethwaiteSolver.Lock")

    static var shared: cubot.ThistlethwaiteSolver {
        lock.sync {
            if let existing = _shared {
                return existing
            }
            _shared = cubot.ThistlethwaiteSolver()
            return _shared!
        }
    }

    static func loadSharedInBackground() {
        DispatchQueue.global(qos: .userInitiated).async {
            _ = shared
        }
    }
}
