import Cubot
import Dispatch

extension cubot.ColorDetector {
    private static var _shared: cubot.ColorDetector?
    private static let lock = DispatchQueue(label: "ColorDetector.Lock")

    static var shared: cubot.ColorDetector {
        lock.sync {
            if let existing = _shared {
                return existing
            }
            _shared = cubot.ColorDetector()
            return _shared!
        }
    }

    static func loadSharedInBackground() {
        DispatchQueue.global(qos: .userInitiated).async {
            _ = shared
        }
    }
}
