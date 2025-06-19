import Cubot
import SwiftUI

extension cubot.Color {
    var asSwiftUIColor: SwiftUI.Color {
        switch self {
        case .white:
            return .white
        case .yellow:
            return .yellow
        case .red:
            return .red
        case .orange:
            return .orange
        case .green:
            return .green
        case .blue:
            return .blue
        @unknown default:
            fatalError()
        }
    }
}
