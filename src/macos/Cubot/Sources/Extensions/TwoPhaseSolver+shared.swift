import Cubot
import Dispatch
import Foundation

extension cubot.TwoPhaseSolver {
    private static var _shared: cubot.TwoPhaseSolver?
    private static let lock = DispatchQueue(label: "TwoPhaseSolver.Lock")

    static var shared: cubot.TwoPhaseSolver {
        lock.sync {
            if let existing = _shared {
                return existing
            }

            if let twophase1URL = Bundle.main.url(
                forResource: "twophase1", withExtension: "pt"),
                let twophase2URL = Bundle.main.url(
                    forResource: "twophase2", withExtension: "pt"),
                let twophase3URL = Bundle.main.url(
                    forResource: "twophase3", withExtension: "pt"),
                let twophase4URL = Bundle.main.url(
                    forResource: "twophase4", withExtension: "pt")
            {
                print("attempting to load the sillliness!")
                _shared = cubot.TwoPhaseSolver(
                    std.string(twophase1URL.path()),
                    std.string(twophase2URL.path()),
                    std.string(twophase3URL.path()),
                    std.string(twophase4URL.path()))
            } else {
                _shared = cubot.TwoPhaseSolver()
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
