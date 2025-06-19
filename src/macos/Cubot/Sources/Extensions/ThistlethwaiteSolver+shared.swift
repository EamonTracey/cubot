import Cubot
import Dispatch
import Foundation

extension cubot.ThistlethwaiteSolver {
    private static var _shared: cubot.ThistlethwaiteSolver?
    private static let lock = DispatchQueue(label: "ThistlethwaiteSolver.Lock")

    static var shared: cubot.ThistlethwaiteSolver {
        lock.sync {
            if let existing = _shared {
                return existing
            }

            if let thistlethwaite1URL = Bundle.main.url(
                forResource: "thistlethwaite1", withExtension: "pt"),
                let thistlethwaite2URL = Bundle.main.url(
                    forResource: "thistlethwaite2", withExtension: "pt"),
                let thistlethwaite3URL = Bundle.main.url(
                    forResource: "thistlethwaite3", withExtension: "pt"),
                let thistlethwaite4URL = Bundle.main.url(
                    forResource: "thistlethwaite4", withExtension: "pt")
            {
                _shared = cubot.ThistlethwaiteSolver(
                    std.string(thistlethwaite1URL.path()),
                    std.string(thistlethwaite2URL.path()),
                    std.string(thistlethwaite3URL.path()),
                    std.string(thistlethwaite4URL.path()))
            } else {
                _shared = cubot.ThistlethwaiteSolver()
            }
            return _shared!
        }
    }

    static func loadSharedInBackground() {
        DispatchQueue.global(qos: .userInitiated).async {
            _ = shared
        }
    }
}
