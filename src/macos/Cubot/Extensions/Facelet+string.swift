import Cubot

extension cubot.Cube.Facelet {
    var string: String {
        switch self {
        case .up: return "Up"
        case .down: return "Down"
        case .right: return "Right"
        case .left: return "Left"
        case .front: return "Front"
        case .back: return "Back"
        default: return ""
        }
    }
}
