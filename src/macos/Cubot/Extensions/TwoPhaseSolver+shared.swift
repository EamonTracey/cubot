import Cubot
import Dispatch

extension cubot.TwoPhaseSolver {
    private static var _shared: cubot.TwoPhaseSolver?
    private static let lock = DispatchQueue(label: "TwoPhaseSolver.Lock")

    static var shared: cubot.TwoPhaseSolver {
        lock.sync {
            if let existing = _shared {
                return existing
            }
            _shared = cubot.TwoPhaseSolver()
            return _shared!
        }
    }

    static func loadSharedInBackground() {
        DispatchQueue.global(qos: .userInitiated).async {
            _ = shared
        }
    }
}
