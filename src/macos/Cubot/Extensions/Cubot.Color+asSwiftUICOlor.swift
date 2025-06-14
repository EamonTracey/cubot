import SwiftUI

import Cubot

extension Cubot.Color {
    var asSwiftUIColor: SwiftUI.Color {
        switch self {
        case .WHITE:
            return .white
        case .YELLOW:
            return .yellow
        case .GREEN:
            return .green
        case .BLUE:
            return .blue
        case .RED:
            return .red
        case .ORANGE:
            return .orange
        @unknown default:
            fatalError()
        }
    }
}
